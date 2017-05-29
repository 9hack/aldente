#include <game/game_state.h>
#include <input/modal_input.h>
#include "build.h"
#include "audio/audio_manager.h"

bool BuildPhase::is_menu = true;

void BuildPhase::s_setup() {
    transition_after(60, proto::Phase::DUNGEON);
    ready_conn = events::player_ready_event.connect([&](int player_id) {
        context.ready_flags[player_id] = true;
    });

    s_verify_and_build_conn = events::build::s_verify_and_build.connect([&](proto::Construct& c) {
        Player* player = GameState::players[c.player_id()];
        assert(player);
        auto type = static_cast<ConstructType>(c.type());
        auto& construct = Constructs::CONSTRUCTS.at(type);

        // If player can afford to purchase, try to build on the tile.
        if (player->can_afford(construct.cost)) {
            events::build::s_try_build_event(c, [&]() {

                // If build successful, deduct the cost from player's funds.
                player->s_modify_stats([construct](PlayerStats& stats) {
                    stats.add_coins(-construct.cost);
                });
            });
        }
    });

    for (int id : context.player_ids) {
        context.ready_flags[id] = false;
    }

    for (auto & p : GameState::players) {
        Player* player = p.second;
        player->set_start_position({ 2.f, 0, 1.f + p.first });
        player->reset_position();
    }

    // Resets all game objects
    for (auto & kv : GameObject::game_objects) {
        kv.second->s_reset();
    }
}

void BuildPhase::c_setup() {
    events::build::start_build_event();
    events::build::select_grid_return_event();
    is_menu = true;

    joystick_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).sticks.connect([&](const events::StickData &d) {

        // Left stick for UI Grid Movement
        if (d.input == events::STICK_LEFT) {
            // Movement axes.
            Direction dir;
            bool moving = true;

            if (d.state.first == events::INPUT_ANALOG_LEVELS && d.state.second == 0)
                dir = Direction::RIGHT;
            else if (d.state.first == -events::INPUT_ANALOG_LEVELS && d.state.second == 0)
                dir = Direction::LEFT;
            else if (d.state.second == events::INPUT_ANALOG_LEVELS && d.state.first == 0)
                dir = Direction::DOWN;
            else if (d.state.second == -events::INPUT_ANALOG_LEVELS && d.state.first == 0)
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

    button_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).buttons.connect([&](const events::ButtonData &d) {
        Direction dir;
        bool d_pad = false;

        if (d.state == 0) return;
        switch (d.input) {
            case events::BTN_A: {
                if (is_menu)
                    events::build::select_grid_confirm_event();
                else
                    events::build::build_grid_place_event();
                break;
            }
            case events::BTN_B: {
                if (!is_menu) {
                    events::build::select_grid_return_event();
                    is_menu = true;
                }
                break;
            }
            case events::BTN_START: {
                proto::ClientMessage msg;
                msg.set_ready_request(context.player_id);
                events::client::send(msg);
                break;
            }
            case events::BTN_UP: {
                dir = Direction::UP;
                d_pad = true;
                break;
            }
            case events::BTN_RIGHT: {
                dir = Direction::RIGHT;
                d_pad = true;
                break;
            }
            case events::BTN_DOWN: {
                dir = Direction::DOWN;
                d_pad = true;
                break;
            }
            case events::BTN_LEFT: {
                dir = Direction::LEFT;
                d_pad = true;
                break;
            }
            case events::BTN_LB: {
                if (!is_menu)
                    events::build::c_rotate_preview_event(false);
                break;
            }
            case events::BTN_RB: {
                if (!is_menu)
                    events::build::c_rotate_preview_event(true);
                break;
            }
            default:
                break;
        }

        if (d_pad) {
            if (is_menu)
                events::build::select_grid_move_event(dir);
            else
                events::build::build_grid_move_event(dir);
        }
    });

    c_preview_conn = events::build::construct_selected_event.connect([&](ConstructType type) {
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[context.player_id]);
        assert(player);
        int cost = Constructs::CONSTRUCTS.at(type).cost;
        bool afford = player->can_afford(cost);

        // Create a construct preview, tinted green/red based on whether or not player can afford.
        events::build::c_construct_preview_event(type, afford);
    });

    c_check_funds_conn = events::build::c_check_funds_event.connect([&](ConstructType type) {
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[context.player_id]);
        assert(player);
        int cost = Constructs::CONSTRUCTS.at(type).cost;
        bool afford = player->can_afford(cost);

        // Switch to building on the 3D grid, since player can afford to build this construct.
        if (player->can_afford(cost)) {
            is_menu = false;
            events::build::construct_selected_event(type);
        }
    });

    // Play music
    events::music_event(events::AudioData{ AudioManager::BUILD_MUSIC, 30, true });

    // Resets game objects on client side
    for (auto & kv : GameObject::game_objects) {
        if (dynamic_cast<Player*>(kv.second))
            kv.second->disable();
        kv.second->c_reset();
    }
}

proto::Phase BuildPhase::s_update() {
    bool all_ready = true;
    for (auto const &kv : context.ready_flags) {
        if (!kv.second) {
            all_ready = false;
            break;
        }
    }

    if (all_ready)
        return proto::Phase::DUNGEON;
    else
        return next;
}

void BuildPhase::s_teardown() {
    cancel_clock_every();
    ready_conn.disconnect();
    s_verify_and_build_conn.disconnect();

    events::build::end_build_event();
}

void BuildPhase::c_teardown() {
    joystick_conn.disconnect();
    button_conn.disconnect();
    c_preview_conn.disconnect();
    c_check_funds_conn.disconnect();

    events::build::end_build_event();
}
