#include "audio_manager.h"

#include <boost/range.hpp>
#include <boost/filesystem.hpp>

#include <math.h>
#include <string>
#include <iostream>

#define SOUND_DIR_PATH "assets/audio/sound/"

#define SFX_DECREASE_DISTANCE_THRESHOLD 7
#define SFX_DECREASE_RATIO 0.1

const std::string AudioManager::BUILD_MUSIC = "assets/audio/music/mikoto.wav";
const std::string AudioManager::DUNGEON_MUSIC = "assets/audio/music/k_theme.wav";

const std::string AudioManager::BUILD_CONFIRM_SOUND = "assets/audio/sound/build_confirm.wav";
const std::string AudioManager::FOOTSTEPS_SOUND = "assets/audio/sound/footsteps.wav";
const std::string AudioManager::ARROW_SWOOSH_SOUND = "assets/audio/sound/arrow_swoosh.wav";

const float AudioManager::SFX_DECREASE_COEFFICIENT = log(SFX_DECREASE_RATIO) / SFX_DECREASE_DISTANCE_THRESHOLD;

AudioManager::AudioManager() : muted(true), max_music_volume(100.0), max_sound_effects_volume(100.0) {
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

        music.setVolume(max_music_volume);
        music.setLoop(d.loop);

        if (muted) return;
        
        music.play();
    });

    events::sound_effects_event.connect([&](const events::AudioData &d) {
        std::string filename = d.filename;

        float vol = volumeByDistance(d.distance);
        std::cerr << "Volume of " << filename << " is " << vol << " with distance " << d.distance << std::endl;
        sounds[filename].setVolume(vol);
        sounds[filename].setLoop(d.loop);

        if (d.loop) {
            sounds[filename].play();
            sounds[filename].pause();
        }

        if (muted) return;

        sounds[filename].play();
    });

    events::stop_sound_effects_event.connect([&](const std::string filename) {
        sounds[filename].stop();
    });

    events::toggle_mute_event.connect([&]() {
        muted = !muted;
        if (muted) {
            music.pause();

            // Mute all sound effects
            for (auto &sound : sounds) {
                sound.second.pause();
            }
        } else {
            music.play();

            // Play looped sound effects that has been paused
            for (auto &sound : sounds) {
                if (sound.second.getStatus() == sf::SoundSource::Status::Paused) {
                    sound.second.play();
                }
            }
        }
    });
}

void AudioManager::loadSounds() {
    // Load sound effects into memory
    boost::filesystem::path path = boost::filesystem::path(SOUND_DIR_PATH);
    
    for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(path), {})) {
        std::string filename = SOUND_DIR_PATH + entry.path().filename().string();
        if (!sound_buffers[filename].loadFromFile(filename)) {
            std::cerr << "AudioManager: Cannot open " << filename << std::endl;
        }
        sounds[filename] = sf::Sound(sound_buffers[filename]);
    }
}

/*
 * Model is a decreasing exponential function:
 * Volume heard = (Max volume) * e^(decreasing coefficient * distance between objects)
 * where decreasing coefficient = log(SFX_DECREASE_RATIO) / SFX_DECREASE_DISTANCE_THRESHOLD and < 0
 */
float AudioManager::volumeByDistance(float distance) {
    return max_sound_effects_volume * exp(SFX_DECREASE_COEFFICIENT * distance);
}
