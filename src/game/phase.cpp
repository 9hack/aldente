#include "phase.h"
#include "game_objects/player.h"

bool BuildPhase::is_menu = true;

void BuildPhase::setup() {
}

void BuildPhase::client_setup() {
    events::build::start_build_event();

    joystick_conn = events::stick_event.connect([&](events::StickData d) {

        // Left stick for UI Grid Movement
        if (d.input == events::STICK_LEFT) {
            // Movement axes.
            Direction dir;
            bool moving = true;
            if (d.state.first == events::INPUT_ANALOG_LEVELS)
                dir = Direction::RIGHT;
            else if (d.state.first == -events::INPUT_ANALOG_LEVELS)
                dir = Direction::LEFT;
            else if (d.state.second == events::INPUT_ANALOG_LEVELS)
                dir = Direction::DOWN;
            else if (d.state.second == -events::INPUT_ANALOG_LEVELS)
                dir = Direction::UP;
            else
                moving = false;

            if (moving) {
                if (is_menu)
                    events::build::select_grid_move_event(dir);
                else
                    events::build::build_grid_move_event(dir);
            }
        }
        else if (d.input == events::STICK_RIGHT) {
            events::build::pan_camera_event(d.state);
        }
    });

    button_conn = events::button_event.connect([&](events::ButtonData d) {
        // A button pressed.
        if (d.input == events::BTN_A && d.state == 1) {
            if (is_menu)
                events::build::select_grid_confirm_event();
            else
                events::build::build_grid_place_event();
            is_menu = false;
        }
        // B button pressed.
        else if (d.input == events::BTN_B && d.state == 1) {
            events::build::select_grid_return_event();
            is_menu = true;
        }
    });
}

Phase* BuildPhase::update() {
    return nullptr;
}

void BuildPhase::teardown() {
}

void BuildPhase::client_teardown() {
    joystick_conn.disconnect();
    button_conn.disconnect();

    events::build::end_build_event();
}

void DungeonPhase::setup() {
    collision_conn = events::dungeon::network_collision_event.connect([&](int obj_id) {
        context.collisions.insert(obj_id);
    });

    interact_conn = events::dungeon::network_interact_event.connect([&](int obj_id) {
        context.interacts.insert(obj_id);
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
    for (auto o : GameObject::game_objects) {
        if (dynamic_cast<Player*>(o.second))
            context.updated_objects.insert(o.second);
    }

    events::dungeon::update_state_event(context.updated_objects, context.collisions, context.interacts);
    context.updated_objects.clear();
    context.collisions.clear();
    context.interacts.clear();

    return nullptr;
}

void DungeonPhase::teardown() {
    collision_conn.disconnect();
    interact_conn.disconnect();
}

void DungeonPhase::client_teardown() {
    joystick_conn.disconnect();
    button_conn.disconnect();
}
