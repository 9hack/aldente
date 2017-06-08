#pragma once

/*
* Ball Class:
* These class represents the ball that players stand on
* in the "sumo" minigame.
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"
class Ball : public GameObject {
private: 
    Player *player;
    glm::vec3 prev_player_vec;
    bool first;
        
    std::vector<std::string> BALL_MODELS = { "slime_blue", "lizard_ball", "cat_ball", "tomato_ball" };
public:
    Ball(int id = 0);

    void c_update_this() override;

    void c_set_player(Player *to_set);
};