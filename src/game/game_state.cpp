#include "game_state.h"

Context GameState::context;
MenuPhase GameState::menu_phase(context);
BuildPhase GameState::build_phase(context);
DungeonPhase GameState::dungeon_phase(context);

Phase* GameState::curr_phase;
std::map<int, Player*> GameState::players;

Physics GameState::physics;
SceneManager GameState::scene_manager;
MainScene GameState::main_scene;
StartScene GameState::start_scene;
int GameState::num_players = 0;
bool GameState::is_server = true;

void GameState::setup(bool is_server) {
    GameState::is_server = is_server;
    scene_manager.add_scene(&start_scene);
    scene_manager.add_scene(&main_scene);

    // TODO: shouldn't need physics on client, but this is needed to create tiles & their rigid bodies.
    // physics.set_scene(&start_scene);
    // scene_manager.set_current_scene(&start_scene, is_server);

    events::menu::end_menu_event.connect([&]() {
        std::cerr << "menu phase ended\n";
        physics.set_scene(&main_scene);
        scene_manager.set_current_scene(&main_scene, GameState::is_server);

        for (auto & kv : players) {
            kv.second->add_rigidbody();
        }
    });

    if (is_server) {
        //scene_manager.get_current_scene()->s_setup();
        physics.set_scene(&main_scene);
        main_scene.s_setup();
        physics.set_scene(&start_scene);
        start_scene.s_setup();

        // Client of given connection id wishes to join the game.
        // For now, allow more than 4 players to join the game.
        events::menu::request_join_event.connect([](int conn_id) {
            proto::JoinResponse resp;
            resp.set_status(true);
            resp.set_id(conn_id);

            Player* player;
            if (players.find(conn_id) != players.end()) {
                // If this is a reconnection, reuse the existing player.
                player = players[conn_id];
                resp.set_obj_id(player->get_id());
            } else {
                // Need to create a new player for the client.
                player = s_add_player(conn_id);
                player->add_rigidbody();
                resp.set_obj_id(player->get_id());
                context.player_ids.push_back(player->get_id());
                context.ready_flags[player->get_id()] = false;
                players[conn_id] = player;
                num_players++;
            }

            resp.set_model_index(player->c_get_model_index());
            events::menu::respond_join_event(conn_id, resp);
        });
    }
    else {
        //scene_manager.get_current_scene()->c_setup();
        physics.set_scene(&main_scene);
        main_scene.c_setup();
        physics.set_scene(&start_scene);
        start_scene.c_setup();

        events::menu::spawn_existing_player_event.connect([](int id, int model_index) {
            c_add_player(id, model_index, false);
        });
    }

    scene_manager.set_current_scene(&start_scene, is_server);
}

void GameState::s_update() {
    assert(curr_phase);
    set_phase(curr_phase->s_update());
    scene_manager.get_current_scene()->s_update();
}

void GameState::c_update() {
    scene_manager.get_current_scene()->c_update();
    curr_phase->c_update();
}

void GameState::set_phase(Phase* phase) {
    // If no change to phase, don't teardown and setup.
    if (phase == curr_phase) return;

    if (is_server) {
        if (curr_phase)
            curr_phase->s_teardown();
        curr_phase = phase;
        curr_phase->s_setup();
    }
    else {
        if (curr_phase)
            curr_phase->c_teardown();
        curr_phase = phase;
        curr_phase->c_setup();
    }
}

void GameState::set_phase(proto::Phase phase) {

    switch (phase) {
    case proto::Phase::NOOP:
        return;
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
        // FIXME(metakirby5)
        break;
    }

    if (is_server) {
        // Announce the phase
        proto::ServerMessage phase_announce;
        phase_announce.set_phase_update(phase);
        events::server::announce(phase_announce);
    }
}

Player* GameState::s_add_player(int conn_id) {
    // For now, only create players on the start scene.
    //assert(scene_manager.get_current_scene() == &start_scene);
    Player *player = new Player();

    // TODO: determine where each player starts based on client id. 
    // For now, players 1-4 start at (2, 2), (2, 3), (2, 4), (2, 5) respectively.
    player->set_start_position({ (2 * (conn_id - 1)), 0, 0 });
    player->s_set_model_index(conn_id % Player::PLAYER_MODELS.size());
    player->reset_position();

    start_scene.objs.push_back(player);
    main_scene.objs.push_back(player);

    return player; //start_scene.s_spawn_player(conn_id);
}

Player* GameState::c_add_player(int obj_id, int model_index, bool is_client) {
    // For now, only create players on the start scene.
    //assert(scene_manager.get_current_scene() == &start_scene);

    // NOTE: players joining after menu phase ends will produce undefined behavior.
    // DEBUG: discarding obj_id

    if (is_client)
        context.player_id = obj_id;

    Player *player = new Player(obj_id);
    player->c_setup_player_model(model_index);
    
    start_scene.objs.push_back(player);
    main_scene.objs.push_back(player);

    return player; // start_scene.c_spawn_player(obj_id, model_index);
}
