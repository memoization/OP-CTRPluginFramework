#include "types.h"
#include <3ds.h>
#include "CTRPluginFramework/System/Time.hpp"

#define TICKS_PER_SEC       268123480
#define SYSCLOCK_SOC       (16756991)
#define SYSCLOCK_ARM9      (SYSCLOCK_SOC * 8)
#define SYSCLOCK_ARM11     (SYSCLOCK_ARM9 * 2)
#define SYSCLOCK_ARM11_NEW (SYSCLOCK_ARM11 * 3)

#define CPU_TICKS_PER_MSEC (SYSCLOCK_ARM11 / 1000.0)
#define CPU_TICKS_PER_USEC (SYSCLOCK_ARM11 / 1000000.0)

namespace CTRPluginFramework
{
    const Time Time::Zero;

    Time::Time(void) : _microseconds(0)
    {

    }

    float   Time::AsSeconds(void) const
    {
        return (_microseconds / 1000000.f);
    }

    int     Time::AsMilliseconds(void) const
    {
        return static_cast<int>(_microseconds / 1000);
    }

    s64     Time::AsMicroseconds(void) const
    {
        return (_microseconds);
    }

    Time::Time(s64 microseconds) : _microseconds(microseconds)
    {
    }

    Time    Time::GetCurrent(void)
    {
        return Microseconds(svcGetSystemTick() / CPU_TICKS_PER_USEC);
    }

    Time    Seconds(float amount)
    {
        return (Time(static_cast<s64>(amount * 1000000)));
    }

    Time    Milliseconds(int amount)
    {
        return (Time(static_cast<s64>(amount) * 1000));
    }

    Time    Microseconds(s64 amount)
    {
        return (Time(amount));
    }

    bool operator ==(Time left, Time right)
    {
        return (left.AsMicroseconds() == right.AsMicroseconds());
    }

    bool operator !=(Time left, Time right)
    {
        return (left.AsMicroseconds() != right.AsMicroseconds());
    }

    bool operator <(Time left, Time right)
    {
        return (left.AsMicroseconds() < right.AsMicroseconds());
    }

    bool operator >(Time left, Time right)
    {
        return (left.AsMicroseconds() > right.AsMicroseconds());
    }

    bool operator <=(Time left, Time right)
    {
        return (left.AsMicroseconds() <= right.AsMicroseconds());
    }

    bool operator >=(Time left, Time right)
    {
        return (left.AsMicroseconds() >= right.AsMicroseconds());
    }

    Time operator -(Time right)
    {
        return (Microseconds(-right.AsMicroseconds()));
    }

    Time operator +(Time left, Time right)
    {
        return (Microseconds(left.AsMicroseconds() + right.AsMicroseconds()));
    }

    Time& operator +=(Time& left, Time right)
    {
        return (left = left + right);
    }

    Time operator -(Time left, Time right)
    {
        return (Microseconds(left.AsMicroseconds() - right.AsMicroseconds()));
    }

    Time& operator -=(Time& left, Time right)
    {
        return left = left - right;
    }

    Time operator *(Time left, float right)
    {
        return (Seconds(left.AsSeconds() * right));
    }

    Time operator *(Time left, s64 right)
    {
        return (Microseconds(left.AsMicroseconds() * right));
    }

    Time operator *(float left, Time right)
    {
        return (right * left);
    }

    Time operator *(s64 left, Time right)
    {
        return (right * left);
    }

    Time& operator *=(Time& left, float right)
    {
        return (left = left * right);
    }

    Time& operator *=(Time& left, s64 right)
    {
        return (left = left * right);
    }

    Time operator /(Time left, float right)
    {
        return Seconds(left.AsSeconds() / right);
    }

    Time operator /(Time left, s64 right)
    {
        return (Microseconds(left.AsMicroseconds() / right));
    }

    Time& operator /=(Time& left, float right)
    {
        return (left = left / right);
    }

    Time& operator /=(Time& left, s64 right)
    {
        return (left = left / right);
    }

    float operator /(Time left, Time right)
    {
        return (left.AsSeconds() / right.AsSeconds());
    }

    Time operator %(Time left, Time right)
    {
        return (Microseconds(left.AsMicroseconds() % right.AsMicroseconds()));
    }

    Time& operator %=(Time& left, Time right)
    {
        return (left = left % right);
    }
}
