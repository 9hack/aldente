#pragma once

#include <glm/glm.hpp>
#include "game_objects/game_object.h"
#include "btBulletDynamicsCommon.h"

class Construct : public GameObject {
protected:
    btRigidBody *rigidBody;
    btBoxShape *box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
public:
    Construct(Scene *scene, int x, int z);
    virtual void interact_trigger() = 0;
};

class Crate : public Construct {
public:
    Crate(Scene *scene, int x, int z);
    void interact_trigger();
};
