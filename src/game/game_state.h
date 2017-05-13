#pragma once

#include "phase.h"
#include "game_objects/player.h"
#include "physics.h"
#include "scene_manager.h"
#include <map>
#include <unordered_set>

class GameState {
public:
    static MenuPhase menu_phase;
    static BuildPhase build_phase;
    static DungeonPhase dungeon_phase;
    static MinigamePhase minigame_phase;

    static Phase* curr_phase;
    static SceneManager scene_manager;
    static Physics physics;

    static std::map<int, Player*> players;
    static std::unordered_set<GameObject*> updated_objects;
    static std::unordered_set<int> collisions;

    // Initializes the game state with the given start phase.
    static void init(Phase* phase);

    // Performs graphical setup on the current scene. Only for the client.
    static void graphical_setup();

    // Update state given input. Called in the game loop.
    // NOTE: not yet implemented; need input to pass in
    static void update();
    static void client_update();

    // Force a phase change to the specified phase.
    static void set_phase(Phase* phase);
    static void set_phase(proto::Phase phase);

    // Adds a player to the main scene with given id.
    // Specify if we should set up graphics for the player model (client-side).
    static Player* add_new_player();
    static Player* add_existing_player(int obj_id);
private:
    static MainScene testScene;
    static int num_players;
};