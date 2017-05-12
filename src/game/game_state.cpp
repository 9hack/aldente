#include "game_state.h"
#include "events.h"

MenuPhase GameState::menu_phase;
BuildPhase GameState::build_phase;
DungeonPhase GameState::dungeon_phase;
MinigamePhase GameState::minigame_phase;
Phase* GameState::curr_phase;
std::map<int, Player*> GameState::players;
std::unordered_set<int> GameState::collisions;

Physics GameState::physics;
SceneManager GameState::scene_manager;
MainScene GameState::testScene;
int GameState::num_players = 0;

void GameState::init(Phase* phase) {
    curr_phase = phase;
    curr_phase->setup();

    physics.set_scene(&testScene);
    scene_manager.set_current_scene(&testScene);

    events::menu::request_join_event.connect([](int conn_id) {
        proto::JoinResponse resp;
        resp.set_status(num_players < 4);
        resp.set_id(conn_id);

        if (num_players < 4) {
            resp.set_obj_id(add_player(conn_id, -1, false)->get_id());
        }
        events::menu::respond_join_event(conn_id, resp);
    });

    events::menu::spawn_new_player_event.connect([](proto::Player & p) {
        add_player(p.id(), -1, false);
    });

    events::menu::spawn_existing_player_event.connect([](proto::Player & p, int obj_id) {
        add_player(p.id(), obj_id, true);
    });

    events::dungeon::network_collision_event.connect([&](int obj_id) {
        collisions.insert(obj_id);
    });
}

void GameState::graphical_setup() {
    scene_manager.get_current_scene()->graphical_setup();
}

void GameState::update() {
    Phase* next_phase = curr_phase->update();
    if (next_phase) {
        curr_phase->teardown();
        curr_phase = next_phase;
        curr_phase->setup();
    }
    physics.update();
    scene_manager.get_current_scene()->update();

    if (curr_phase == &dungeon_phase) {
        events::dungeon::network_positions_event(players, collisions);
        collisions.clear();
    }
}

void GameState::client_update() {
    scene_manager.get_current_scene()->client_update();
}

void GameState::set_phase(Phase* phase) {
    curr_phase->teardown();
    curr_phase = phase;
    curr_phase->setup();
}

void GameState::set_phase(proto::Phase phase) {
    switch (phase) {
    case proto::Phase::MENU:
        GameState::set_phase(&GameState::menu_phase);
        break;
    case proto::Phase::BUILD:
        GameState::set_phase(&GameState::build_phase);
        break;
    case proto::Phase::DUNGEON:
        GameState::set_phase(&GameState::dungeon_phase);
        break;
    case proto::Phase::MINIGAME:
        GameState::set_phase(&GameState::minigame_phase);
        break;
    }
}


Player* GameState::add_player(int conn_id, int obj_id, bool graphical) {
    assert(players.find(conn_id) == players.end());
    
    // For now, only create players on the main scene.
    assert(scene_manager.get_current_scene() == &testScene);
    Player* player = testScene.spawn_player(conn_id, obj_id, graphical);
    players[conn_id] = player;
    num_players++;
    return player;
}
