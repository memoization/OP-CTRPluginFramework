#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

namespace CTRPluginFramework
{
    namespace Hid
    {
        static bool     g_useExpad = false;

        int     PatchCepdThread(void);

        void    Initialize(void)
        {
            bool useExpad = ExtraPadReader::InstallHooks();

            PadReader::InstallHooks();
            TouchPanelReader::InstallHooks();

            if (!useExpad && SystemImpl::IsNew3DS)
            {
                // Init IRRST if we're on N3DS
                RightStick::Initialize();
            }
            else // Search for cepd thread start
            {
                if (PatchCepdThread())
                    OSD::Notify("Couldn't find cepdthreadfunc");
                else
                    g_useExpad = true;

            }
        }

        void    ReadLatest(void)
        {
            if (g_useExpad)
                Hid::ExtraPadReader::ReadLatest();
            else
                Hid::PadReader::ReadLatest();
            Hid::TouchPanelReader::ReadLatest();
        }
    }
}
