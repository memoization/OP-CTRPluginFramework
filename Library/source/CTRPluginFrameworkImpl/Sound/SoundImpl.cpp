
#include "CTRPluginFrameworkImpl/Sound/SoundImpl.hpp"
#include "CTRPluginFramework/System/File.hpp"
#include <cstring>

namespace CTRPluginFramework
{
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