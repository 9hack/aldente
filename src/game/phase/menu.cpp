#include <game/game_state.h>
#include "menu.h"

void MenuPhase::s_setup() {
}

void MenuPhase::c_setup() {
    button_conn = events::button_event.connect([&](events::ButtonData d) {
        if (d.state == 0) return;
        switch (d.input) {
            case events::BTN_START: {
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
    return next;
}

void MenuPhase::c_update() {
}

void MenuPhase::s_teardown() {
    events::menu::end_menu_event();
}

void MenuPhase::c_teardown() {
    button_conn.disconnect();

    events::menu::end_menu_event();
}
