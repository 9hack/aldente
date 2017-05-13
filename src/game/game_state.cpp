#include "game_state.h"
#include "events.h"

MenuPhase GameState::menu_phase;
BuildPhase GameState::build_phase;
DungeonPhase GameState::dungeon_phase;
MinigamePhase GameState::minigame_phase;
Phase* GameState::curr_phase;
std::map<int, Player*> GameState::players;
std::unordered_set<GameObject*> GameState::updated_objects;
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
            Player* player = add_new_player();
            resp.set_obj_id(player->get_id());
            players[conn_id] = player;
            num_players++;
        }
        events::menu::respond_join_event(conn_id, resp);
    });

    events::menu::spawn_existing_player_event.connect([](int id) {
        add_existing_player(id);
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
        for (auto o : GameObject::game_objects) {
            if (o.second->tag == Tag::PLAYER)
                updated_objects.insert(o.second);
        }
        events::dungeon::network_positions_event(updated_objects, collisions);
        updated_objects.clear();
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

Player* GameState::add_new_player() {
    // For now, only create players on the main scene.
    assert(scene_manager.get_current_scene() == &testScene);
    return testScene.spawn_new_player();
}

Player* GameState::add_existing_player(int obj_id) {    
    // For now, only create players on the main scene.
    assert(scene_manager.get_current_scene() == &testScene);
    return testScene.spawn_existing_player(obj_id);
}
