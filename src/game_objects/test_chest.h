#pragma once

#include "game_objects/game_object.h"

class TestChest : public GameObject {
public:
    void open_chest();
private:
    virtual void update() override;
};