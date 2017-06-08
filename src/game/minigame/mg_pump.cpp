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
    // Set up players
    for (int id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->enable();

        player->get_rigid()->setLinearFactor(btVector3(0, 0, 0));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        player->transform.set_position(player_start_pos[count++]);
    }

    GameState::set_scene(context.minigame_scenes["pump"]);
}

void PumpMG::s_teardown() {
    for (int id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->get_rigid()->setLinearFactor(btVector3(1, 0, 1));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
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
    GameState::set_scene(context.minigame_scenes["pump"]);
}

void PumpMG::c_teardown() {
}

bool PumpMG::is_finished() {
    return false;
}