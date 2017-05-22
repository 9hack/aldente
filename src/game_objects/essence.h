#pragma once

#include "game_object.h"
#include "events.h"

#define DEFAULT_ESSENSE_VAL 10

/*
    Dropped objects that players can pick up for currency
*/
class Essence : public GameObject {
public:
    Essence(int id = 0);

    void set_value(int val) { value = val; };

    void c_update_state(float x, float z, float wx, float wz, bool enab) override;

    void s_on_collision(GameObject *other) override;
    void c_on_collision(int type) override;

    void setup_model() override;

    void disappear();

private:
    btSphereShape *hit_sphere = new btSphereShape(0.2f);

    int value; // Amount this essence can provide

    std::function<void()> cancel_fade;
};