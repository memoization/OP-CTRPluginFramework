#include "CTRPluginFramework/Sound.hpp"
#include "CTRPluginFrameworkImpl/Sound.hpp"

namespace CTRPluginFramework
{
    static inline void* ImplToPublic(SoundImpl* impl)
    {
        return static_cast<void*>(impl);
    }

    static inline SoundImpl* PublicToImpl(void* publ)
    {
        return static_cast<SoundImpl*>(publ);
    }

    Sound::Sound(const File& bcwavFile, int maxSimultPlays)
    {
        _soundImpl = ImplToPublic(new SoundImpl(bcwavFile, maxSimultPlays));
    }

    Sound::Sound(const void* bcwavBuffer, int maxSimultPlays)
    {
        _soundImpl = ImplToPublic(new SoundImpl(bcwavBuffer, maxSimultPlays));
    }

    Sound::LoadStatus Sound::GetLoadStatus()
    {
        return static_cast<Sound::LoadStatus>(PublicToImpl(_soundImpl)->GetLoadStatus());
    }

    void Sound::SetVolume(float volume)
    {
        PublicToImpl(_soundImpl)->SetVolume(volume);
    }

    float Sound::GetVolume()
    {
        return PublicToImpl(_soundImpl)->GetVolume();
    }

    void Sound::SetPan(float pan)
    {
        PublicToImpl(_soundImpl)->SetPan(pan);
    }

    float Sound::GetPan()
    {
        return PublicToImpl(_soundImpl)->GetPan();
    }

    u32 Sound::GetChannelAmount()
    {
        return PublicToImpl(_soundImpl)->GetChannelAmount();
    }

    bool Sound::IsLooped()
    {
        return PublicToImpl(_soundImpl)->IsLooped();
    }

    bool Sound::Play()
    {
        return Play(0);
    }

    bool Sound::Play(int monoChannel)
    {
        return Play(monoChannel, -1);
    }

    bool Sound::Play(int leftEarChannel, int rightEarChannel)
    {
        return PublicToImpl(_soundImpl)->Play(leftEarChannel, rightEarChannel);
    }

    void Sound::Stop() {
        Stop(-1);
    }

    void Sound::Stop(int monoChannel)
    {
        Stop(monoChannel, -1);
    }

    void Sound::Stop(int leftEarChannel, int rightEarChannel) {
        PublicToImpl(_soundImpl)->Stop(leftEarChannel, rightEarChannel);
    }

    Sound::~Sound() {
        delete PublicToImpl(_soundImpl);
    }
}