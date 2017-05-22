#include <string>
#include <iostream>
#include "audio_manager.h"

const std::string AudioManager::BUILD_MUSIC = "assets/audio/music/mikoto.wav";
const std::string AudioManager::DUNGEON_MUSIC = "assets/audio/music/k_theme.wav";

const std::string AudioManager::BUILD_CONFIRM_SOUND = "assets/audio/sound/build_confirm.wav";

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
        music.setLoop(true);
        if (!muted)
            std::cerr << "Playing music" << std::endl;
            music.setVolume(d.volume);
            music.play();
    });

    events::sound_effects_event.connect([&](const events::AudioData &d) {
        std::string filename = d.filename;
        //if (!muted) {
            std::cerr << "Start sound effects event." << std::endl;
            //sf::SoundBuffer soundBuffer;
            if (!build_confirm_buffer->loadFromFile("assets/audio/music/k_theme.wav")) {
                std::cerr << "AudioManager: Cannot open" << BUILD_CONFIRM_SOUND << std::endl;;
            }
            sf::Sound sound(*build_confirm_buffer);
            std::cerr << "Sound buffer at " << sound.getBuffer() << " with volume " << sound.getVolume() << std::endl;
            sound.play();
            std::cerr << "Status right after play is " << sound.getStatus() << std::endl;
            std::cerr << "End sound effects event." << std::endl;
        //}

        /*
        if (!sound.openFromFile(filename)) {
            std::cerr << "AudioManager: Cannot open " << filename << std::endl;;
        }

        std::cerr << "Playing sound" << std::endl;
        sound.setVolume(d.volume);
        sound.play();
        */
    });

    events::toggle_mute_event.connect([&]() {
        std::cerr << "Toggling mute" << std::endl;
        muted = !muted;
        if (muted) {
            music.pause();
        } else {
            music.play();
        }
    });
}

void AudioManager::loadSounds() {
    // Load sound effects into memory
    build_confirm_buffer = new sf::SoundBuffer();
    if (!build_confirm_buffer->loadFromFile(BUILD_CONFIRM_SOUND)) {
        std::cerr << "AudioManager: Cannot open" << BUILD_CONFIRM_SOUND << std::endl;;
    }
    build_confirm_sound.setBuffer(*build_confirm_buffer);
    sounds[BUILD_CONFIRM_SOUND] = build_confirm_sound;
}
