#ifndef CTRPLUGINFRAMEWORK_SOUND_SOUNDENGINE_HPP
#define CTRPLUGINFRAMEWORK_SOUND_SOUNDENGINE_HPP

#include "CTRPluginFramework/Sound/Sound.hpp"

namespace CTRPluginFramework {
    class SoundEngine
    {

    public:
        enum class Event {
            CURSOR = 0,
            ACCEPT,
            CANCEL,
            SELECT,
            DESELECT,

            NUM_EVENTS
        };

        static bool RegisterMenuSoundEvent(Event eventType, Sound& sound);

        static Sound& GetMenuSoundEvent(SoundEngine::Event eventType);

        static bool PlayMenuSound(Event eventType);

        static void StopMenuSound(Event eventType);

        static void DeRegisterMenuSoundEvent(Event eventType);

    };
}

#endif