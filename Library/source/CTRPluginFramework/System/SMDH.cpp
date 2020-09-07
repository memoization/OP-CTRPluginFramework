#include "CTRPluginFramework/System/SMDH.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFrameworkImpl/System/ProcessImpl.hpp"

#include <3ds/result.h>
#include <3ds/services/cfgu.h>
#include <3ds/util/utf.h>
#include <3ds/services/fs.h>

namespace CTRPluginFramework {

char SMDH::ShortDescription[SHORT_DESCRIPTOR]{0};
char SMDH::LongDescription[LONG_DESCRIPTOR]{0};

void SMDH::Init(void)
{
    ProcessImpl::PatchFSAccess(true);

    SMDH::Read(Process::GetTitleID());

    ProcessImpl::PatchFSAccess(false);
}

void SMDH::ExtractSMDHTitle(SMDHTitle& title)
{
    utf16_to_utf8((u8*)ShortDescription, title.ShortDescription, SHORT_DESCRIPTOR - 1);
    utf16_to_utf8((u8*)LongDescription, title.LongDescription, LONG_DESCRIPTOR - 1);
}

void SMDH::Read(u64 tid, SMDH* smdh)
{ // https://github.com/J-D-K/JKSM/blob/0ac6c08e6f8c78f55bcb642fb2e3c55500307b6a/src/data.cpp#L244
    static const u32 filePath[] = {0x0, 0x0, 0x2, 0x6E6F6369, 0x0}; // "icon" in ASCII
    Handle handle;
    u32 read = 0;

    u32 low = static_cast<u32>(tid);
    u32 high = static_cast<u32>(tid >> 32);
    u8  media = FS_MediaType(MEDIATYPE_SD); // Add check for nand titles?

    u32 archPath[] = {low, high, media, 0};

    //FS_Path binArchPath = {PATH_EMPTY, 1, ""};
    FS_Path binArchPath = {PATH_BINARY, 0x10, archPath};
    FS_Path binFilePath = {PATH_BINARY, 0x14, filePath};

    if(R_SUCCEEDED(FSUSER_OpenFileDirectly(&handle, ARCHIVE_SAVEDATA_AND_CONTENT, binArchPath, binFilePath, FS_OPEN_READ, 0)))
    {
        if(smdh) // Read full SMDH
            FSFILE_Read(handle, &read, 0, (void*)smdh, sizeof(SMDH));
        else // Read and extract SMDHTitle
        {
            SMDH* smdh = new SMDH;

            // cant set offset for some reason
            if(R_SUCCEEDED(FSFILE_Read(handle, &read, 0, (void*)smdh, sizeof(SMDH))))
            {
                CFG_Language systemLanguage;
                CFGU_GetSystemLanguage(reinterpret_cast<u8*>(&systemLanguage));
                ExtractSMDHTitle(smdh->Titles[systemLanguage]);
            }

            delete smdh;
        }

        FSFILE_Close(handle);
    }
}
} // CTRPluginFramework
