#pragma once

/*
* Platform class: For all your platform needs
*/

#include "../game_object.h"
#include "btBulletDynamicsCommon.h"

enum PlatformShape {
    SQUARE,
    CIRCLE,
};

class Platform : public GameObject {
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(10, 0.5f, 5));
    btCylinderShape *hit_cylinder = new btCylinderShape(btVector3(6, 0.5f, 6));
    int type;
public:
    Platform(PlatformShape shape);

    void setup_model() override;
};
