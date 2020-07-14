#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"
#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"
#include "CTRPluginFramework/System/Hook.hpp"
#include "CTRPluginFramework/Utils.hpp"

#include <3ds.h>
#include <vector>

#define THREADVARS_MAGIC  0x21545624 // !TV$

namespace CTRPluginFramework
{
    namespace Hid
    {
        static Hook     g_cepdHook;

        static u32   CepdThreadFunc(u32 arg)
        {
            u32 *tls = (u32* )getThreadLocalStorage();

            *tls = THREADVARS_MAGIC;

            return HookContext::GetCurrent().OriginalFunction<u32>(arg);
        }

        // Patch to prevent locking the extrapad sampler thread
        int  PatchCepdThread(void)
        {
            u32 address = 0;

            const std::vector<std::vector<u32>> patterns =
            {
                { 0xE59F0024, 0xE92D4008, 0xE5901024, 0xE2800014, 0xE58D1000, 0xE890000F }
            };

            for (const auto& pattern : patterns)
            {
                address = Utils::Search<u32>(0x00100000, Process::GetTextSize(), pattern);

                if (address)
                    break;
            }

            if (!address)
                return 1;

            g_cepdHook
                .InitializeForMitm(address + 4, (u32)CepdThreadFunc)
                .Enable();

            return 0;
        }
    }
}
