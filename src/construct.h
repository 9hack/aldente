#pragma once

#include <glm/glm.hpp>
#include "game_objects/game_object.h"

class Construct : public GameObject {
public:
    Construct(Scene *scene, int x, int z);
};

class Crate : public Construct {
public:
    Crate(Scene *scene, int x, int z);
};
