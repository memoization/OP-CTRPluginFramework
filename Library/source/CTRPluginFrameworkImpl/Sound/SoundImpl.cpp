
#include "CTRPluginFrameworkImpl/Sound.hpp"
#include "CTRPluginFramework/System/File.hpp"
#include <cstring>

namespace CTRPluginFramework
{
    std::vector<Sound> SoundEngineImpl::menuSounds;
    Sound SoundEngineImpl::fallbackSound;

    static const char* defaultSoundFiles[(u32)SoundEngine::Event::NUM_EVENTS] =
    {
        "sound_cursor.bcwav",
        "sound_accept.bcwav",
        "sound_cancel.bcwav",
        "sound_select.bcwav",
        "sound_deselect.bcwav"
    };

    void SoundEngineImpl::NotifyAptEvent(APT_HookType event)
    {
        cwavNotifyAptEvent(event);
    }

    void SoundEngineImpl::SetVaToPaConvFunction(vaToPaCallback_t function)
    {
        cwavSetVAToPACallback(function);
    }

    bool SoundEngineImpl::RegisterMenuSoundEvent(SoundEngine::Event eventType, Sound& sound)
    {
        if (sound.GetLoadStatus() == Sound::LoadStatus::SUCCESS && eventType < SoundEngine::Event::NUM_EVENTS)
        {
            menuSounds[(u32)eventType] = sound;
            return true;
        }
        return false;
    }

    Sound& SoundEngineImpl::GetMenuSoundEvent(SoundEngine::Event eventType)
    {
        if (eventType < SoundEngine::Event::NUM_EVENTS)
            return  menuSounds[(u32)eventType];
        else
            return fallbackSound;
    }

    bool SoundEngineImpl::PlayMenuSound(SoundEngine::Event eventType)
    {
        if (eventType < SoundEngine::Event::NUM_EVENTS)
            return menuSounds[(u32)eventType].Play();
        return false;
    }

    void SoundEngineImpl::StopMenuSound(SoundEngine::Event eventType)
    {
        if (eventType < SoundEngine::Event::NUM_EVENTS)
            menuSounds[(u32)eventType].Stop();
    }

    void SoundEngineImpl::DeRegisterMenuSoundEvent(SoundEngine::Event eventType)
    {
        if (eventType < SoundEngine::Event::NUM_EVENTS)
            menuSounds[(u32)eventType] = Sound();
    }

    void SoundEngineImpl::InitializeMenuSounds()
    {
        menuSounds.resize((u32)SoundEngine::Event::NUM_EVENTS);

        for (u32 i = 0; i < (u32)SoundEngine::Event::NUM_EVENTS; i++)
        {
            Sound curr(defaultSoundFiles[i], 3);
            if (curr.GetLoadStatus() == Sound::LoadStatus::SUCCESS)
                RegisterMenuSoundEvent((SoundEngine::Event)i, curr);
        }
    }

    void SoundEngineImpl::ClearMenuSounds()
    {
        for (u32 i = 0; i < (u32)SoundEngine::Event::NUM_EVENTS; i++)
            menuSounds[i] = Sound();
    }

    SoundImpl::SoundImpl(const std::string& bcwavFile, int maxSimultPlays)
    {
        File cwavFile(bcwavFile, File::READ);
        std::memset(&_cwav, 0, sizeof(CWAV));
        _dataBuffer = nullptr;

        if (!cwavFile.IsOpen())
        {
            _cwav.loadStatus = CWAV_FILE_OPEN_FAILED;
            return;
        }

        u32 fileSize = cwavFile.GetSize();

        _dataBuffer = static_cast<void *>(::operator new(fileSize, std::nothrow));
        if (_dataBuffer == nullptr)
        {
            _cwav.loadStatus = CWAV_FILE_TOO_LARGE;
            return;
        }

        cwavFile.Seek(0);
        cwavFile.Read(_dataBuffer, fileSize);

        cwavLoad(&_cwav, _dataBuffer, maxSimultPlays);
    }

    SoundImpl::SoundImpl(const u8* bcwavBuffer, int maxSimultPlays)
    {
        _dataBuffer = nullptr;
        cwavLoad(&_cwav, bcwavBuffer, maxSimultPlays);
    }

    cwavLoadStatus_t SoundImpl::GetLoadStatus()
    {
        return _cwav.loadStatus;
    }

    void SoundImpl::SetVolume(float volume)
    {
        _cwav.volume = volume;
    }

    float SoundImpl::GetVolume()
    {
        return _cwav.volume;
    }

    void SoundImpl::SetPan(float pan)
    {
        _cwav.monoPan = pan;
    }

    float SoundImpl::GetPan()
    {
        return _cwav.monoPan;
    }

    u32 SoundImpl::GetChannelAmount()
    {
        return _cwav.numChannels;
    }

    bool SoundImpl::IsLooped()
    {
        return _cwav.isLooped;
    }

    bool SoundImpl::Play(int leftEarChannel, int rightEarChannel)
    {
        return cwavPlay(&_cwav, leftEarChannel, rightEarChannel);
    }

    void SoundImpl::Stop(int leftEarChannel, int rightEarChannel)
    {
        cwavStop(&_cwav, leftEarChannel, rightEarChannel);
    }

    SoundImpl::~SoundImpl()
    {
        cwavFree(&_cwav);
        if (_dataBuffer)
            ::operator delete(_dataBuffer);
    }
}