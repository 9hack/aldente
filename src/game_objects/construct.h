#pragma once

#include <glm/glm.hpp>
#include "game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"

class Construct : public GameObject {
protected:
    btBoxShape *box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
public:
    Construct(int x, int z);
    virtual void interact_trigger() = 0;
};

class Crate : public Construct {
public:
    Crate(int x, int z);
    void interact_trigger();
};
