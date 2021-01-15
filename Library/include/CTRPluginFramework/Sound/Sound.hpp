#ifndef CTRPLUGINFRAMEWORK_SOUND_SOUND_HPP
#define CTRPLUGINFRAMEWORK_SOUND_SOUND_HPP

#include "types.h"
#include <string>

namespace CTRPluginFramework
{
    class Sound
    {
    public:
        enum class LoadStatus {
            NOT_ALLOCATED = 0, ///< Data is not allocated and cannot be used.
            SUCCESS = 1, ///< Loaded properly and is ready to play.
            INVALID_ARGUMENT = 2, ///< An invalid argument was passed to the constructor.
            FILE_OPEN_FAILED = 3, ///< Failed to open the specified file.
            FILE_TOO_LARGE = 4, ///< The file is too large to fit in the available memory.
            UNKNOWN_FILE_FORMAT = 5, ///< The specified file is not a valid CWAV file.
            INVAID_INFO_BLOCK = 6, ///< The INFO block in the CWAV file is invalid or not supported.
            INVAID_DATA_BLOCK = 7, ///< The DATA block in the CWAV file is invalid or not supported.
            UNSUPPORTED_AUDIO_ENCODING = 8 ///< The audio encoding is not supported.
        };
        Sound();
        // Warning: copies share the same auidio buffer
        Sound(const Sound& sound);
        Sound(Sound&& sound) noexcept;
        Sound& operator=(const Sound& sound);
        Sound& operator=(Sound&& sound) noexcept;

        Sound(const std::string& bcwavFile, int maxSimultPlays = 1);

        Sound(const u8* bcwavBuffer, int maxSimultPlays = 1);

        LoadStatus GetLoadStatus();

        void SetVolume(float volume);

        float GetVolume();

        void SetPan(float pan);

        float GetPan();

        u32 GetChannelAmount();

        bool IsLooped();

        bool Play();

        bool Play(int monoChannel);

        bool Play(int leftEarChannel, int rightEarChannel);

        void Stop();

        void Stop(int monoChannel);

        void Stop(int leftEarChannel, int rightEarChannel);

        ~Sound();

    private:
        void*   _soundImpl;
    };
}

#endif