#pragma once

#include <glm/glm.hpp>
#include "game_object.h"

class Construct : public GameObject {

};

class Crate : public Construct {
public:
    Crate(int x, int z);
};
