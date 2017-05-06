#pragma once

#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"

// Utility class for converting between Bullet vectors/matrices to OpenGL vectors/matrices
namespace util_bt {
    glm::vec3 convert_vec3(btVector3 vec);
    glm::mat4 convert_trans(btTransform trans);
}