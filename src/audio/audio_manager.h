#pragma once

#include <SFML/Audio.hpp>
#include <string.h>
#include <unordered_map>
#include "events.h"

class AudioManager {
public:
    static const std::string BUILD_MUSIC;
    static const std::string DUNGEON_MUSIC;

    static const std::string BUILD_CONFIRM_SOUND;
    static const std::string ARROW_SWOOSH_SOUND;

    std::unordered_map<std::string, sf::Sound> sounds;
    std::unordered_map<std::string, sf::SoundBuffer> sound_buffers;

    AudioManager();
private:
    static const float SFX_DECREASE_COEFFICIENT;

    sf::Music music;
    bool muted;
    float max_music_volume, max_sound_effects_volume;

    void loadSounds();
    float volumeByDistance(float distance);
};