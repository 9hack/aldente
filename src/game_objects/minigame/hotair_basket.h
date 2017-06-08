#pragma once

/*
* Basket class: Basket part of the hotair balloon minigame
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"

class HotairBasket : public GameObject {
public:
    HotairBasket(int id);

    void setup_model() override;
};