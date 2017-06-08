#pragma once

/*
* Pump class: It's a pump
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"

class Pump : public GameObject {
private:
    int player_id;
public:
    Pump(int id);

    void setup_model() override;

    void set_player_id(int to_set) { player_id = to_set; };
};