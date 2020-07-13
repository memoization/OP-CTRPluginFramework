#include "types.h"
#include "3ds.h"

#include "CTRPluginFramework/System/Controller.hpp"
#include "CTRPluginFramework/System/Touch.hpp"

#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"

namespace CTRPluginFramework
{
    using namespace Hid;

    bool        Touch::IsDown(void)
    {
        return TouchPanelStatus::Get().touch;//(Controller::GetKeysDown() & Key::Touchpad);
    }

    UIntVector  Touch::GetPosition(void)
    {
        const TouchPanelStatus& tps = TouchPanelStatus::Get();

        return (UIntVector(tps.x, tps.y));
    }
}
