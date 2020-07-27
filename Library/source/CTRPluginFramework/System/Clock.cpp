#include "types.h"

#include "CTRPluginFramework/System/Time.hpp"
#include "CTRPluginFramework/System/Clock.hpp"

namespace CTRPluginFramework
{
    Clock::Clock(void) : _startTime(Time::GetCurrent())
    {

    }

    Clock::Clock(Time time) : _startTime{ time }
    {
    }

    Time    Clock::GetElapsedTime(void) const
    {
        return (Time::GetCurrent() - _startTime);
    }

    bool    Clock::HasTimePassed(Time time) const
    {
        return (GetElapsedTime() >=  time);
    }

    Time    Clock::Restart(void)
    {
        Time now = Time::GetCurrent();

        Time ret = now - _startTime;

        _startTime = now;
        return (ret);
    }
}
