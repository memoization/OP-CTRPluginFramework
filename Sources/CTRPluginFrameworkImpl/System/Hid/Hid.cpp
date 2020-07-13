#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"

namespace CTRPluginFramework
{
    namespace Hid
    {
        void    Initialize(void)
        {
            PadReader::InstallHooks();
            TouchPanelReader::InstallHooks();
            ExtraPadReader::InstallHooks();
            // Search for cepd function
        }
    }
}
