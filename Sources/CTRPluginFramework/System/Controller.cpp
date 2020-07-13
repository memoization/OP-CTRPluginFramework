#include "types.h"
#include <3ds.h>
#include "CTRPluginFramework/System/Controller.hpp"
#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFrameworkImpl/System/Hid/HidApi.hpp"

namespace CTRPluginFramework
{
    namespace ControllerImpl
    {
        u32     _keysDown = 0;
        u32     _keysHeld = 0;
        u32     _keysReleased = 0;
        u32     _counter = 0;
        u32     _gameCounter = 0;
    }

    using namespace ControllerImpl;

    u32     Controller::GetKeysDown(void)
    {
        return (_keysDown | _keysHeld);
    }

    u32     Controller::GetKeysReleased(void)
    {
        return _keysReleased;
    }

    bool    Controller::IsKeyDown(Key key)
    {
        return (_keysHeld & (u32)key) != 0;
    }

    bool    Controller::IsKeyPressed(Key key)
    {
        return (_keysDown & (u32)key) != 0;
    }

    bool    Controller::IsKeyReleased(Key key)
    {
        return (_keysReleased & (u32)key) != 0;
    }

    bool    Controller::IsKeysDown(u32 keys)
    {
        return ((_keysHeld & keys) == keys);
    }

    bool    Controller::IsKeysPressed(u32 keys)
    {
        if ((_keysDown & keys) && ((_keysHeld | _keysDown) & keys) == keys)
            return (true);
        return (false);
    }

    bool    Controller::IsKeysReleased(u32 keys)
    {
        if ((_keysReleased & keys) == keys)
            return (true);
        return (false);
    }

    void    Controller::Update(void)
    {
        /*hidScanInput();
        _keysDown = hidKeysDown();
        _keysHeld = hidKeysHeld();
        _keysReleased = hidKeysUp();*/
        if (Process::IsPaused())
        {
            Hid::PadReader::ReadLatest();
            Hid::TouchPanelReader::ReadLatest();
        }
        else
        {
            _counter++;
        }

    }
}
