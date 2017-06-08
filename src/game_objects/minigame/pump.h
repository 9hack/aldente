#pragma once

/*
* Pump class: It's a pump
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"

enum PumpColor {
    RED, BLUE,
};

class Pump : public GameObject {
private:
    int player_id;
    int color;
public:
    Pump(int id, PumpColor to_set);

    void setup_model() override;

    void c_set_player_id(int to_set) { player_id = to_set; };
};