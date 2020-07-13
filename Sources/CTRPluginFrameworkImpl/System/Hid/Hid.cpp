#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"

namespace CTRPluginFramework
{
    namespace Hid
    {
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
        }
    }
}
