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

        music.setVolume(d.volume);
        music.setLoop(d.loop);

        if (!muted) {
            music.play();
        }
    });

    events::sound_effects_event.connect([&](const events::AudioData &d) {
        std::string filename = d.filename;

        sounds[filename]->setVolume(d.volume);
        sounds[filename]->setLoop(d.loop);

        if (!muted) {
            sounds[filename]->play();
        }
        else if (muted && d.loop) {
            // Change state to paused so that when audio is un-muted, the looped sound will play
            sounds[filename]->play();
            sounds[filename]->pause();
        }
    });

    events::stop_sound_effects_event.connect([&](const std::string filename) {
        sounds[filename]->stop();
    });

    events::toggle_mute_event.connect([&]() {
        muted = !muted;
        if (muted) {
            music.pause();

            // Mute all sound effects
            for (auto it = sounds.begin(); it != sounds.end(); it++) {
                it->second->pause();
            }
        }
        else {
            music.play();

            // Play looped sound effects that has been paused
            for (auto it = sounds.begin(); it != sounds.end(); it++) {
                if (it->second->getStatus() == sf::SoundSource::Status::Paused) {
                    it->second->play();
                }
            }
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
