#ifndef CTRPLUGINFRAMEWORK_SOUND_SOUND_IMPL_HPP
#define CTRPLUGINFRAMEWORK_SOUND_SOUND_IMPL_HPP

#include "CTRPluginFramework/Sound.hpp"
#include "cwav.h"

namespace CTRPluginFramework
{
    class SoundEngineImpl
    {
    public:
        static void NotifyAptEvent(APT_HookType event);

        static void SetVaToPaConvFunction(vaToPaCallback_t function);
    };
    class SoundImpl
    {
    public:

        SoundImpl(const File& bcwavFile, int maxSimultPlays = 1);

        SoundImpl(const void* bcwavBuffer, int maxSimultPlays = 1);

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

    private:
        CWAV    _cwav;
        void*   _dataBuffer;
    };
}

#endif