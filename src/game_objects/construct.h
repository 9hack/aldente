#pragma once

#include <glm/glm.hpp>
#include "game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"

// Note: Opengl has the x and z axis as its ground plane

class Construct : public GameObject {
protected:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
public:
    Construct(int x, int z);
    Construct(int x, int z, int id);

    virtual void setup_model() = 0;
    virtual void update_this() override = 0;
};

class Crate : public Construct {
public:
    Crate(int x, int z);
    Crate(int x, int z, int id);
    void setup_model();
    void update_this() override;
    void interact_trigger() override;
};
