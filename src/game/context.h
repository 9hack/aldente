#pragma once

#include "game_objects/game_object.h"
#include <set>

class Context {
public:
    // Server
    std::set<GameObject*> updated_objects;
    std::set<std::pair<int, int>> collisions;
    std::set<std::pair<int, int>> interacts;
    std::vector<int> player_ids;
    std::unordered_map<int, bool> ready_flags; // <Player id, if ready>

    // Client
    int player_id;
    bool player_finished;
};