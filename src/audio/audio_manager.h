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

    std::unordered_map<std::string, sf::Sound> sounds;

    AudioManager();
private:
    sf::Music music;
    bool muted;
    
    // Sound effects
    sf::SoundBuffer * build_confirm_buffer;
    sf::Sound build_confirm_sound;
    //sf::Music sound;

    void loadSounds();
};