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
    for (int id : context.player_ids) {
        Player* player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);

        // Set player start location here
        player->set_position({ 0, 0, 0 });
        player->enable();
    }

    // Enable gravity on players
    for (int id : context.player_ids) {
        Player *player = dynamic_cast<Player*>(GameObject::game_objects[id]);
        assert(player);
        player->get_rigid()->setLinearFactor(btVector3(1, 1.5f, 1));
        player->get_rigid()->setLinearVelocity(btVector3(0, 0, 0));
        glm::vec3 pos = player->transform.get_position();
        pos.y = 1.f;
        player->set_speed(1.0f);
        player->set_position(pos);
    }

    //GameState::set_scene(context.minigame_scenes["penguin"]);
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
    //GameState::set_scene(context.minigame_scenes["penguin"]);
}

void PumpMG::c_teardown() {
}

bool PumpMG::is_finished() {
}