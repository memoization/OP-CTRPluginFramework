#include "types.h"
#include <3ds.h>
#include "csvc.h"
#include "CTRPluginFramework/System/System.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"

namespace CTRPluginFramework
{
    void    (*System::OnAbort)(void) = nullptr;

    bool    System::IsNew3DS(void)
    {
        return (SystemImpl::IsNew3DS);
    }

    bool    System::IsCitra(void)
    {
        return (SystemImpl::IsCitra);
    }

    LanguageId  System::GetSystemLanguage(void)
    {
        return (static_cast<LanguageId>(SystemImpl::Language));
    }

    bool    System::IsConnectedToInternet(void)
    {
        u32 out = 0;
        u8  *wifiLevel = (u8 *)0x1FF81066;

        if (R_FAILED(ACU_GetWifiStatus(&out)))
            out = 0;

        return (out != 0 && *wifiLevel != 0);
    }

    /// Retrieves the major version from a packed system version.
#define GET_VERSION_MAJOR(version)    ((version) >>24)

    /// Retrieves the minor version from a packed system version.
#define GET_VERSION_MINOR(version)    (((version)>>16)&0xFF)

    /// Retrieves the revision version from a packed system version.
#define GET_VERSION_REVISION(version) (((version)>> 8)&0xFF)

    static inline bool isServiceUsable(const char *name)
    {
        bool r;
        return R_SUCCEEDED(srvIsServiceRegistered(&r, name)) && r;
    }

    bool    System::CfwIsLuma3DS(const u8 major, const u8 minor, const u8 revision)
    {
        if (SystemImpl::CFWVersion == 0 || !major)
            return (false);

        u8  _major = GET_VERSION_MAJOR(SystemImpl::CFWVersion);
        u8  _minor = GET_VERSION_MINOR(SystemImpl::CFWVersion);
        u8  _revision = GET_VERSION_REVISION(SystemImpl::CFWVersion);

        if (_major < major) return (false);
        if (_minor < minor) return (false);
        return (_revision >= revision);
    }

    void System::ToggleWireless(bool enabled)
    {
        if (isServiceUsable("nwm::EXT"))
        {
            nwmExtInit();
            NWMEXT_ControlWirelessEnabled(enabled);
            nwmExtExit();
        }
    }

    void System::ResetWireless()
    {
        if(R_FAILED(acInit()))
            return;

        acuConfig config = {0};
        Handle connectEvent = 0;
        svcCreateEvent(&connectEvent, RESET_ONESHOT);
        ACU_ConnectAsync(&config, connectEvent);
        acExit();
        svcCloseHandle(connectEvent);
    }

    void    System::Reboot(void)
    {
        svcKernelSetState(7);
        for (;;);
    }
}
