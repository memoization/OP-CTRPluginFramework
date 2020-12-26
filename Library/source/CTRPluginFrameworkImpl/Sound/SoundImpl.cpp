
#include "CTRPluginFrameworkImpl/Sound.hpp"
#include "CTRPluginFramework/System/File.hpp"

namespace CTRPluginFramework
{
    void SoundEngineImpl::NotifyAptEvent(APT_HookType event)
    {
        cwavNotifyAptEvent(event);
    }

    void SoundEngineImpl::SetVaToPaConvFunction(vaToPaCallback_t function)
    {
        cwavSetVAToPACallback(function);
    }

    SoundImpl::SoundImpl(const File& bcwavFile, int maxSimultPlays)
    {
        if (!bcwavFile.IsOpen())
        {
            _cwav.loadStatus = CWAV_FILE_OPEN_FAILED;
            return;
        }

        u32 fileSize = bcwavFile.GetSize();

        _dataBuffer = static_cast<void *>(::operator new(fileSize, std::nothrow));
        if (_dataBuffer == nullptr)
        {
            _cwav.loadStatus = CWAV_FILE_TOO_LARGE;
            return;
        }

        bcwavFile.Seek(0);
        bcwavFile.Read(_dataBuffer, fileSize);

        cwavLoadFromBuffer(&_cwav, _dataBuffer, maxSimultPlays);
    }

    SoundImpl::SoundImpl(const void* bcwavBuffer, int maxSimultPlays)
    {
        _dataBuffer = nullptr;
        cwavLoadFromBuffer(&_cwav, bcwavBuffer, maxSimultPlays);
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

    SoundImpl::~SoundImpl() {
        cwavFree(&_cwav);
        if (_dataBuffer)
            ::operator delete(_dataBuffer);
    }
}