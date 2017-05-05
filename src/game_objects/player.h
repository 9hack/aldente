#pragma once

#include "game_objects/game_object.h"

class Player : public GameObject {
public:
    void start_walk();
    void stop_walk();
private:
    virtual void update() override;
};