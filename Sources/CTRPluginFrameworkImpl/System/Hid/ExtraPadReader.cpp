#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/Utils.hpp"
#include "CTRPluginFramework/Graphics/OSD.hpp"

namespace CTRPluginFramework
{
    namespace ControllerImpl
    {
        extern u32     _keysDown;
        extern u32     _keysHeld;
        extern u32     _keysReleased;
    }

    namespace Hid
    {
        using ReadLatestFunc = bool (*)(ExtraPadReader*, ExtraPadStatus* status);

        static Hook             g_extraPadReaderHook;
        static ExtraPadStatus   g_extraPadStatus;
        static ReadLatestFunc   ExtraPadReader__ReadLatest = [](ExtraPadReader*, ExtraPadStatus*) { return false; }; // Stub so this funcptr is always set

        static void    UpdateControllerFromGame(ExtraPadStatus* padStatus)
        {
            ControllerImpl::_keysDown = padStatus->trigger;
            ControllerImpl::_keysHeld = padStatus->hold;
            ControllerImpl::_keysReleased = padStatus->release;
        }

        static bool    ExtraPadReaderHookFunc(ExtraPadReader* padReader, ExtraPadStatus* padStatus)
        {
            // Use game function to read inputs
            bool couldRead = ExtraPadReader__ReadLatest(padReader, padStatus);

            if (!couldRead)
                return couldRead;

            g_extraPadStatus = *padStatus;

            // Update controller
            UpdateControllerFromGame(padStatus);

            return couldRead;
        }

        static void    PatchSelect(void)
        {
            u32 address = 0;

            const std::vector<std::vector<u32>> patterns =
            {
                { 0xE2802010, 0xE280100C, 0xE2800008 }
            };

            for (const auto& pattern : patterns)
            {
                address = Utils::Search<u32>(0x00100000, Process::GetTextSize(), pattern);

                if (address)
                    break;
            }

            if (!address)
                return;

            // Apply patch
            *(vu32 *)address = 0xE12FFF1E; // bx lr
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

            // Apply patch
            PatchSelect();

            return true;
        }
    }
}
