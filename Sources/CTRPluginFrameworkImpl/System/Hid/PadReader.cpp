#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/Utils.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

namespace CTRPluginFramework
{
    namespace Hid
    {
        using ReadLatestFunc = bool (*)(PadReader*, PadStatus* status);

        static Hook             g_padReaderHook;
        static PadStatus        g_padStatus;
        static ReadLatestFunc   PadReader__ReadLatest = [](PadReader*, PadStatus*) { return false; }; // Stub so this funcptr is always set

        void    UpdateControllerFromGame(PadStatus* padStatus)
        {
            // Hmm, not sure of the approach yet
        }

        bool    PadReaderHookFunc(PadReader* padReader, PadStatus* padStatus)
        {
            // Use game function to read inputs
            bool couldRead = PadReader__ReadLatest(padReader, padStatus);

            if (!couldRead)
                return couldRead;

            g_padStatus = *padStatus;

            // Update controller
            UpdateControllerFromGame(padStatus);

            return couldRead;
        }

        bool    PadReader::InstallHooks(void)
        {
            u32     address = 0;
            Hook&   hook = g_padReaderHook;

            const std::vector<std::vector<u32>> patterns =
            {
                { 0xE92D4070, 0xE24DD018, 0xE1A04000, 0xE3E00000, 0xE58D0010, 0xE58D0014 },
                { 0xE92D4070, 0xE1A05000, 0xE24DD018, 0xE3E00000, 0xE1A04001 }
            };

            for (const auto& pattern : patterns)
            {
                address = Utils::Search<u32>(0x00100000, Process::GetTextSize(), pattern);

                if (address)
                    break;
            }

            if (!address)
            {
                OSD::Notify("Couldn't find PadReader!");
                return false;
            }

            // Set hook
            g_padReaderHook
                .InitializeForMitm(address, (u32)PadReaderHookFunc)
                .Enable();

            // Set funcptr
            PadReader__ReadLatest = (ReadLatestFunc)g_padReaderHook.GetContext().GetCallCode();

            return true;
        }
    }
}
