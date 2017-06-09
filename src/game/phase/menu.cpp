#include <game/game_state.h>
#include "menu.h"
#include <input/modal_input.h>
#include <util/config.h>

std::string MenuPhase::to_string() {
    return "MAIN MENU";
}

void MenuPhase::s_setup() {
    Config::config->get_value(Config::str_show_dialogues, show_dialogues);

    ready_conn = events::player_ready_event.connect([&](int player_id) {
        // Ready up if not previously ready. Otherwise un-ready up.
        context.ready_flags[player_id] = !context.ready_flags[player_id];

        proto::ServerMessage msg;
        proto::ReadyUpdate* ready = new proto::ReadyUpdate();
        ready->set_player_id(player_id);
        ready->set_ready(context.ready_flags[player_id]);
        msg.set_allocated_ready_update(ready);
        events::server::announce(msg);
    });
}

void MenuPhase::c_setup() {
    button_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).buttons.connect([&](const events::ButtonData &d) {
        if (d.state == 0) return;
        switch (d.input) {
            case events::BTN_START: {
                proto::ClientMessage msg;
                msg.set_ready_request(context.player_id);
                events::client::send(msg);
                if (ready_sound) {
                    events::sound_effects_event(events::AudioData(AudioManager::CHAR_SELECTION_READY_SOUND, false));
                    ready_sound = false;
                } else {
                    events::sound_effects_event(events::AudioData(AudioManager::CHAR_SELECTION_UNREADY_SOUND, false));
                    ready_sound = true;
                }
                break;
            }
            case events::BTN_LB: {
                events::menu::c_cycle_player_model_event(false);
                events::sound_effects_event(events::AudioData(AudioManager::SELECT1_SOUND, false));
                break;
            }
            case events::BTN_RB: {
                events::menu::c_cycle_player_model_event(true);
                events::sound_effects_event(events::AudioData(AudioManager::SELECT1_SOUND, false));
                break;
            }
            default:
                break;
        }
    });

    // Start main menu music
    events::music_event(events::AudioData(AudioManager::MAIN_MENU_MUSIC, true));
}

proto::Phase MenuPhase::s_update() {
    bool all_ready = true;
    for (auto const &kv : context.ready_flags) {
        if (!kv.second) {
            all_ready = false;
            break;
        }
    }

    // Move on to build phase if all players ready, and there's at least one player.
    if (context.player_ids.size() > 0 && all_ready)
        return s_phase_when_done();
    else
        return next;
}

void MenuPhase::c_update() {
}

void MenuPhase::s_teardown() {
    ready_conn.disconnect();

    events::menu::end_menu_event();
}

void MenuPhase::c_teardown() {
    button_conn.disconnect();

    events::menu::end_menu_event();

    events::stop_all_sounds_event();
    events::stop_music_event();

    for (int player_id : context.player_ids) {
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[player_id]);
        player->set_anim_override(false);
        player->set_anim_pause(false);
    }
}

proto::Phase MenuPhase::s_phase_when_done() {
    return show_dialogues ? proto::Phase::BUILD_TUTORIAL : proto::Phase::BUILD;
}