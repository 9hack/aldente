#pragma once

#include "phase.h"
#include "game_objects/player.h"
#include "physics.h"
#include "scene_manager.h"
#include <map>

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

    // Initializes the game state with the given start phase.
    static void init(Phase* phase);

    static void graphical_setup();

    // Update state given input. Called in the game loop.
    // NOTE: not yet implemented; need input to pass in
    static void update();
    static void client_update();

    // Force a phase change to the specified phase.
    static void set_phase(Phase* phase);
    static void set_phase(proto::Phase phase);

    static void add_player(int conn_id, bool graphical);
private:
    static MainScene testScene;
    static int num_players;
};