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
        using ReadLatestFunc = bool (*)(PadReader*, PadStatus* status);

        static Hook             g_padReaderHook;
        static PadStatus        g_padStatus;
        static PadReader *      g_padReader = nullptr;
        static ReadLatestFunc   PadReader__ReadLatest = [](PadReader*, PadStatus*) { return false; }; // Stub so this funcptr is always set

        static void    UpdateControllerFromGame(PadStatus* padStatus)
        {
            ControllerImpl::_keysDown = padStatus->trigger;
            ControllerImpl::_keysHeld = padStatus->hold;
            ControllerImpl::_keysReleased = padStatus->release;
        }

        bool    PadReaderHookFunc(PadReader* padReader, PadStatus* padStatus)
        {
            g_padReader = padReader;

            // Use game function to read inputs
            bool couldRead = PadReader__ReadLatest(padReader, padStatus);

            if (!couldRead)
                return couldRead;

            g_padStatus = *padStatus;

            RightStick::ReadLatest(&g_padStatus);

            // Update controller
            UpdateControllerFromGame(&g_padStatus);

            return couldRead;
        }

        bool    PadReader::ReadLatest(void)
        {
            if (!g_padReader)
                return false;

            bool couldRead = PadReader__ReadLatest(g_padReader, &g_padStatus);

            if (!couldRead)
                return couldRead;

            RightStick::ReadLatest(&g_padStatus);

            UpdateControllerFromGame(&g_padStatus);

            return couldRead;
        }

        static void    PatchSelect(void)
        {
            u32 address = 0;

            const std::vector<std::vector<u32>> patterns =
            {
                { 0xE8900006, 0xE3A0300C, 0xE1D3C002 },
                { 0xE2802008, 0xE2801004 }
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

            // Apply patch
            PatchSelect();

            return true;
        }
    }
}
