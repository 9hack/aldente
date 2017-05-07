#include "game_state.h"
#include "events.h"

MenuPhase GameState::menu_phase;
BuildPhase GameState::build_phase;
DungeonPhase GameState::dungeon_phase;
MinigamePhase GameState::minigame_phase;
Phase* GameState::curr_phase;
std::map<int, Player*> GameState::players;

Physics GameState::physics;
SceneManager GameState::scene_manager;
MainScene GameState::testScene;
int GameState::next_player_id = 1;

void GameState::init(Phase* phase) {
    curr_phase = phase;
    curr_phase->setup();

    physics.set_scene(&testScene);
    scene_manager.set_current_scene(&testScene);

    events::menu::request_join_event.connect([](std::string& name) {
        proto::JoinResponse resp;
        
        if (next_player_id <= 4) {
            add_player();
        }

        for (auto& pair : players) {
            proto::Player* player = resp.add_players();
            player->set_id(pair.first);
            player->set_x(pair.second->transform.get_position().x);
            player->set_z(pair.second->transform.get_position().z);
        }

        resp.set_num_players(next_player_id - 1);
        resp.set_status(next_player_id <= 4);

        std::cerr << "[s] responding with: " << resp.DebugString();
        events::menu::respond_join_event(resp);
    });

    events::menu::spawn_player_event.connect([](proto::Player & player) {
        add_player();
    });
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
}

void GameState::set_phase(Phase* phase) {
    curr_phase->teardown();
    curr_phase = phase;
    curr_phase->setup();
}

void GameState::add_player() {
    Player* player = dynamic_cast<MainScene*>(scene_manager.get_current_scene())->spawn_player();
    players[next_player_id++] = player;
}
