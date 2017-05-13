#pragma once

#include "game_objects/game_object.h"
#include <unordered_set>

class Context {
public:
    std::unordered_set<GameObject*> updated_objects;
    std::unordered_set<int> collisions;
};