#pragma once

#include "game_objects/game_object.h"
#include <unordered_set>

class Context {
public:
    // Server
    std::unordered_set<GameObject*> updated_objects;
    std::set<std::pair<int, int>> collisions;
    std::unordered_set<int> interacts;
    std::vector<int> player_ids;

    // Client
    int player_id;
};