#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/Utils.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

namespace CTRPluginFramework
{
    namespace Hid
    {
        using ReadLatestFunc = bool (*)(ExtraPadReader*, ExtraPadStatus* status);

        static Hook             g_extraPadReaderHook;
        static ReadLatestFunc   ExtraPadReader__ReadLatest = [](ExtraPadReader*, ExtraPadStatus*) { return false; }; // Stub so this funcptr is always set

        static void    UpdateControllerFromGame(ExtraPadStatus* padStatus)
        {
            // Hmm, not sure of the approach yet
        }

        static bool    ExtraPadReaderHookFunc(ExtraPadReader* padReader, ExtraPadStatus* padStatus)
        {
            // Use game function to read inputs
            bool couldRead = ExtraPadReader__ReadLatest(padReader, padStatus);

            if (!couldRead)
                return couldRead;

            // Update controller
            UpdateControllerFromGame(padStatus);

            return couldRead;
        }

        bool    ExtraPadReader::InstallHooks(void)
        {
            u32     address = 0;
            Hook&   hook = g_extraPadReaderHook;

            const std::vector<std::vector<u32>> patterns =
            {
                { 0xE92D4070, 0xE1A05000, 0xE24DD028, 0xE3E00000, 0xE1A04001, 0xE1A01000 }
            };

            for (const auto& pattern : patterns)
            {
                address = Utils::Search<u32>(0x00100000, Process::GetTextSize(), pattern);

                if (address)
                    break;
            }

            if (!address)
            {
                OSD::Notify("Couldn't find ExtraPadReader!");
                return false;
            }

            // Set hook
            g_extraPadReaderHook
                .InitializeForMitm(address, (u32)ExtraPadReaderHookFunc)
                .Enable();

            // Set funcptr
            ExtraPadReader__ReadLatest = (ReadLatestFunc)g_extraPadReaderHook.GetContext().GetCallCode();

            return true;
        }
    }
}
