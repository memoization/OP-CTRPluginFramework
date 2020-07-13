#include <3ds.h>
#include "types.h"
#include "CTRPluginFrameworkImpl/System/Services/Env.hpp"
#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"

namespace CTRPluginFramework
{
    namespace Hid
    {
        namespace RightStick
        {
            static int g_isInit = 0;

            void    Initialize(void)
            {
                const char *port = "ir:rst";

                Handle irrstHandle = 0;
                Result res = 0;

                // Request service.
                if(R_FAILED(res = srvGetServiceHandle(&irrstHandle, port))) return;

                // Add handle to env to prevent ctrulib from starting sampler
                Services::AddServiceToEnv(port, irrstHandle);

                // Init irrst
                irrstInit();

                // Start sampling
                res = IRRST_Initialize(15, 0);

                if (R_FAILED(res))
                {
                    // TODO: some error handling ?
                    return;
                }

                ++g_isInit;
            }

            void    ReadLatest(PadStatus *padStatus)
            {
                if (!g_isInit)
                    return;

                u32     kOld = irrstKeysHeld();

                irrstScanInput();

                u32     keys = irrstKeysHeld();

                padStatus->trigger |= ~kOld & keys;
                padStatus->hold |= keys;
                padStatus->release |= keys & ~kOld;
            }
        }
    }
}
