#include <input/modal_input.h>
#include "mg_pump.h"
#include "events.h"
#include "game/game_state.h"
#include "game_objects/player.h"
#include <iostream>

PumpMG::PumpMG(Context& to_set) : Minigame(to_set) {
    time = PUMPMG_TIME;
    info = MinigameInfo{
        {
            { "xboxControllerButtonA.png", "Pump" },
        }, // input legend
        std::string("Be first to pump up your balloon!"), // Objective
    };

    context = to_set;
}

void PumpMG::s_setup() {
    int count = 0;
    team1.clear();
    team2.clear();

    std::vector<int> player_assignments = context.player_ids;
    std::random_shuffle(player_assignments.begin(), player_assignments.end());

    // Set up players
    bool curr_team = true;
    for (int id : player_assignments) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->enable();

        if (curr_team)
            team1.push_back(player);
        else
            team2.push_back(player);

        player->get_rigid()->setLinearFactor(btVector3(0, 0, 0));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        player->set_ghost(true);
        player->set_position(player_start_pos[count++]);
        player->transform.look_at(glm::vec3(0, 0, 1));

        curr_team = !curr_team;
    }

    GameState::set_scene(context.minigame_scenes["pump"]);
}

void PumpMG::s_teardown() {
    for (int id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->get_rigid()->setLinearFactor(btVector3(1, 0, 1));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        player->set_ghost(false);
        player->set_speed(2.0f);
    }

    /*// Assigns rewards to alive players
    Player *curr_player;
    for (auto const &kv : dead_player_flags) {
        if (!kv.second) {
            curr_player = dynamic_cast<Player*>(GameObject::game_objects[kv.first]);
            assert(curr_player);
            curr_player->s_modify_stats([&, kv](PlayerStats &stats) {
                stats.add_coins(PUMPMG_REWARD);
            });
        }
    }*/
}

void PumpMG::c_setup() {
    int count = 0;

    button_conn = input::ModalInput::get()->with_mode(input::ModalInput::NORMAL).buttons.connect([&](const events::ButtonData &d) {
        if (d.state == 1) {
            switch (d.input) {
            case events::BTN_A:
                // A button pressed.
                events::minigame::c_play_pump_event(context.player_id);
                break;
            default:
                break;
            }
        }
    });

    Player *player = dynamic_cast<Player*>(GameObject::game_objects[context.player_id]);
    assert(player);
    player->set_pump();
    player->set_anim_override(true);

    GameState::set_scene(context.minigame_scenes["pump"]);
}

void PumpMG::c_teardown() {
    Player *player = dynamic_cast<Player*>(GameObject::game_objects[context.player_id]);
    assert(player);
    player->start_walk();
    player->set_anim_override(false);
}

bool PumpMG::is_finished() {
    return false;
}