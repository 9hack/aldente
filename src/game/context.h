#pragma once

#include "game_objects/game_object.h"
#include <unordered_set>

class Context {
public:
    // Server
    std::unordered_set<GameObject*> updated_objects;
    std::unordered_set<int> collisions;
    std::unordered_set<int> interacts;
    std::vector<int> player_ids;
    std::unordered_map<int, bool> ready_flags; // <Player id, if ready>

    // Client
    int player_id;
};