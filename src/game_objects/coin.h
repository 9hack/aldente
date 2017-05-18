#pragma once

#include "game_object.h"

class Coin : public GameObject {
public:
    Coin(int val = 100);

    void set_value(int val) { value = val; };

    virtual void update_this() override;
    virtual void on_collision(GameObject *other) override;
    virtual void setup_model() override;

private:
    btSphereShape *hit_sphere = new btSphereShape(0.2f);

    int value; // Amount of gold this coin can provide

};