#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/Utils.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

namespace CTRPluginFramework
{
    namespace Hid
    {
        using ReadLatestFunc = bool (*)(TouchPanelReader*, TouchPanelStatus* status);

        static Hook             g_touchPanelReaderHook;
        static ReadLatestFunc   TouchPanelReader__ReadLatest = [](TouchPanelReader*, TouchPanelStatus*) { return false; }; // Stub so this funcptr is always set

        static void    UpdateControllerFromGame(TouchPanelStatus* panelStatus)
        {
            // Hmm, not sure of the approach yet
        }

        static bool    TouchPanelReaderHookFunc(TouchPanelReader* panelReader, TouchPanelStatus* panelStatus)
        {
            // Use game function to read inputs
            bool couldRead = TouchPanelReader__ReadLatest(panelReader, panelStatus);

            if (!couldRead)
                return couldRead;

            // Update controller
            UpdateControllerFromGame(panelStatus);

            return couldRead;
        }

        bool    TouchPanelReader::InstallHooks(void)
        {
            u32     address = 0;
            Hook&   hook = g_touchPanelReaderHook;

            const std::vector<std::vector<u32>> patterns =
            {
                { 0xE92D4010, 0xE24DD018, 0xE1A04001, 0xE3E01000 },
            };

            for (const auto& pattern : patterns)
            {
                address = Utils::Search<u32>(0x00100000, Process::GetTextSize(), pattern);

                if (address)
                    break;
            }

            if (!address)
            {
                OSD::Notify("Couldn't find TouchPanelReader!");
                return false;
            }

            // Set hook
            g_touchPanelReaderHook
                .InitializeForMitm(address, (u32)TouchPanelReaderHookFunc)
                .Enable();

            // Set funcptr
            TouchPanelReader__ReadLatest = (ReadLatestFunc)g_touchPanelReaderHook.GetContext().GetCallCode();

            return true;
        }
    }
}
