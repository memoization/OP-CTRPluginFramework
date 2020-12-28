#ifndef CTRPLUGINFRAMEWORK_SOUND_SOUND_IMPL_HPP
#define CTRPLUGINFRAMEWORK_SOUND_SOUND_IMPL_HPP

#include "CTRPluginFramework/Sound.hpp"
#include "cwav.h"
#include <vector>

namespace CTRPluginFramework
{
    class SoundEngineImpl
    {
    public:

        static void NotifyAptEvent(APT_HookType event);

        static void SetVaToPaConvFunction(vaToPaCallback_t function);

        static bool RegisterMenuSoundEvent(SoundEngine::Event eventType, Sound& sound);

        static Sound& GetMenuSoundEvent(SoundEngine::Event eventType);

        static bool PlayMenuSound(SoundEngine::Event eventType);

        static void StopMenuSound(SoundEngine::Event eventType);

        static void DeRegisterMenuSoundEvent(SoundEngine::Event eventType);

        static void InitializeMenuSounds();

        static void ClearMenuSounds();
    private:
        static std::vector<Sound> menuSounds;
        static Sound fallbackSound;
    };
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