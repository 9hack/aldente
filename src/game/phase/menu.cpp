#include <game/game_state.h>
#include "menu.h"
#include <input/modal_input.h>

std::string MenuPhase::to_string() {
    return "MAIN MENU";
}

void MenuPhase::s_setup() {
    ready_conn = events::player_ready_event.connect([&](int player_id) {
        // Ready up if not previously ready. Otherwise un-ready up.
        context.ready_flags[player_id] = !context.ready_flags[player_id];
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
                break;
            }
            case events::BTN_LB: {
                events::menu::c_cycle_player_model_event(false);
                break;
            }
            case events::BTN_RB: {
                events::menu::c_cycle_player_model_event(true);
                break;
            }
            default:
                break;
        }
    });
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
        return proto::Phase::BUILD;
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
}
