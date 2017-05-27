#include <game/game_state.h>
#include "menu.h"

void MenuPhase::s_setup() {
}

void MenuPhase::c_setup() {
}

proto::Phase MenuPhase::s_update() {
    return next;
}

void MenuPhase::c_update() {
}

void MenuPhase::s_teardown() {
}

void MenuPhase::c_teardown() {
}
