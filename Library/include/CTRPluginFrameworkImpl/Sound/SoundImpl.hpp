#ifndef CTRPLUGINFRAMEWORK_SOUND_SOUND_IMPL_HPP
#define CTRPLUGINFRAMEWORK_SOUND_SOUND_IMPL_HPP

#include "cwav.h"
#include <vector>
#include <string>

namespace CTRPluginFramework
{
    class SoundImpl
    {
    public:

        SoundImpl(const std::string& bcwavFile, int maxSimultPlays = 1);

        SoundImpl(const u8* bcwavBuffer, int maxSimultPlays = 1);

        cwavLoadStatus_t GetLoadStatus();

        void SetVolume(float volume);

        float GetVolume();

        void SetPan(float pan);

        float GetPan();

        u32 GetChannelAmount();

        bool IsLooped();

        bool Play(int leftEarChannel, int rightEarChannel);

        void Stop(int leftEarChannel, int rightEarChannel);

        ~SoundImpl();

        s32     _refcount = 1;

    private:
        CWAV    _cwav;
        void*   _dataBuffer;
    };
}

#endif