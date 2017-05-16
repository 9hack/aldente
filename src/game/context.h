#pragma once

#include "game_objects/game_object.h"
#include <unordered_set>

class Context {
public:
    // Server
    std::unordered_set<GameObject*> updated_objects;
    std::unordered_set<int> collisions;
    std::vector<int> player_ids;

    // Client
    int player_id;
};