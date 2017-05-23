#include <string>
#include <iostream>
#include "audio_manager.h"

const std::string AudioManager::BUILD_MUSIC = "assets/audio/music/mikoto.wav";
const std::string AudioManager::DUNGEON_MUSIC = "assets/audio/music/k_theme.wav";

const std::string AudioManager::BUILD_CONFIRM_SOUND = "assets/audio/sound/build_confirm.wav";
const std::string AudioManager::FOOTSTEPS_SOUND = "assets/audio/sound/footsteps.wav";

AudioManager::AudioManager() : muted(true) {
    loadSounds();

    events::music_event.connect([&](const events::AudioData &d) {
        // Stop the current music from playing, if any
        if (music.getStatus() == sf::SoundSource::Status::Playing) {
            music.stop();
        }
        
        std::string filename = d.filename;
        if (!music.openFromFile(filename)) {
            std::cerr << "AudioManager: Cannot open " << filename << std::endl;;
        }

        if (!muted) {
            music.setVolume(d.volume);
            music.setLoop(d.loop);
            music.play();
        }
    });

    events::sound_effects_event.connect([&](const events::AudioData &d) {
        std::string filename = d.filename;
        if (!muted) {
            sounds[filename]->setVolume(d.volume);
            sounds[filename]->setLoop(d.loop);
            sounds[filename]->play();
        }
    });

    events::toggle_mute_event.connect([&]() {
        muted = !muted;
        if (muted) {
            music.pause();
        } else {
            music.play();
        }
    });
}

void AudioManager::loadSound(sf::SoundBuffer * soundBuffer, std::string filename) {
    if (!soundBuffer->loadFromFile(filename)) {
        std::cerr << "AudioManager: Cannot open" << filename << std::endl;;
    }
}

void AudioManager::loadSounds() {
    // Load sound effects into memory

    // BUILD_CONFIRM_SOUND
    loadSound(&build_confirm_buffer, BUILD_CONFIRM_SOUND);
    build_confirm_sound.setBuffer(build_confirm_buffer);
    sounds[BUILD_CONFIRM_SOUND] = &build_confirm_sound;

    // FOOTSTEPS_SOUND
    loadSound(&footsteps_buffer, FOOTSTEPS_SOUND);
    footsteps_sound.setBuffer(footsteps_buffer);
    sounds[FOOTSTEPS_SOUND] = &footsteps_sound;
}
