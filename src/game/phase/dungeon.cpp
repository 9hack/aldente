#include "dungeon.h"
#include "game_objects/player.h"

void DungeonPhase::setup() {
    collision_conn = events::dungeon::network_collision_event.connect([&](int obj_id) {
        context.collisions.insert(obj_id);
    });
}

void DungeonPhase::client_setup() {
    joystick_conn = events::stick_event.connect([&](events::StickData d) {
        // Left stick
        if (d.input == events::STICK_LEFT) {
            events::dungeon::network_player_move_event(d);
        }
    });

    button_conn = events::button_event.connect([&](events::ButtonData d) {
        // A button pressed.
        if (d.input == events::BTN_A && d.state == 1) {
            events::dungeon::player_interact_event();
        }
    });
}

Phase* DungeonPhase::update() {
    // Send the position and orientation of the specified game objects.
    // Currently sending all Player objects.
    for (auto const & o : GameObject::game_objects) {
        if (dynamic_cast<Player*>(o.second))
            context.updated_objects.insert(o.second);
    }
    events::dungeon::network_positions_event(&context);

    return nullptr;
}

void DungeonPhase::teardown() {
    collision_conn.disconnect();
}

void DungeonPhase::client_teardown() {
    joystick_conn.disconnect();
    button_conn.disconnect();
}