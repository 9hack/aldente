#pragma once

#include "game_objects/game_object.h"

class TestChest : public GameObject {
public:
    void open_chest();
private:
    virtual void update_this() override;
    virtual void setup_model() override {};
};