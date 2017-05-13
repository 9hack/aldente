#include "network_manager.h"
#include "util/config.h"
#include "game_objects/player.h"
#include "game/game_state.h"
#include <GLFW/glfw3.h>
#include <unordered_set>

void NetworkManager::disconnect() {
    if (!io_service.stopped())
        io_service.stop();
    if (service_thread) {
        service_thread->join();
        delete service_thread;
    }
}

void NetworkManager::run_service() {
    while (service_thread && !io_service.stopped()) {
        try {
            io_service.poll();
        } catch (std::exception & ex) {
            // Silently ignore errors rather than crash. Shouldn't happen.
            std::cerr << "run_service error: " << ex.what() << "\n";
        }
    }
}

void ServerNetworkManager::connect() {
    register_listeners();
    service_thread = new boost::thread(&NetworkManager::run_service, this);
}

void ServerNetworkManager::register_listeners() {
    // Menu phase.
    events::menu::respond_join_event.connect([&](int conn_id, proto::JoinResponse& resp) {
        proto::ServerMessage msg;
        msg.set_allocated_join_response(new proto::JoinResponse(resp));
        server.send_to(conn_id, msg);
    });

    // Build phase.
    events::build::respond_build_event.connect([&](proto::Construct& c) {
        proto::ServerMessage msg;
        msg.set_allocated_build_update(new proto::Construct(c));
        server.send_to_all(msg);
    });

    // Dungeon phase.
    events::dungeon::network_positions_event.connect([&](std::unordered_set<GameObject*> updated, std::unordered_set<int> collisions) {
        proto::ServerMessage msg;
        proto::GameState* state = new proto::GameState();
        for (auto & obj : updated) {
            proto::GameObject* go = state->add_objects();
            go->set_id(obj->get_id());
            if (obj->tag == Tag::PLAYER)
                go->set_type(proto::GameObject::Type::GameObject_Type_PLAYER);
            else if (obj->tag == Tag::GOAL)
                go->set_type(proto::GameObject::Type::GameObject_Type_GOAL);
            go->set_x(obj->transform.get_position().x);
            go->set_z(obj->transform.get_position().z);
            go->set_wx(obj->direction.x);
            go->set_wz(obj->direction.z);
        }

        for (int obj_id : collisions) {
            state->add_collisions(obj_id);
        }

        msg.set_allocated_state_update(state);
        server.send_to_all(msg);
    });
}

void ServerNetworkManager::update() {
    for (auto& client : server.read_all_messages()) {
        for (auto& msg : client.second) {
            switch (msg.message_type_case()) {
            case proto::ClientMessage::MessageTypeCase::kBuildRequest: {
                proto::Construct construct = msg.build_request();
                events::build::try_build_event(construct);
                break;
            }
            case proto::ClientMessage::MessageTypeCase::kMoveRequest: {
                proto::StickData stick = msg.move_request();
                if (stick.input() == proto::StickData::STICK_LEFT)
                    GameState::players[stick.id()]->prepare_movement(stick.x(), stick.y());
                break;
            }
            case proto::ClientMessage::MessageTypeCase::kPhaseRequest: {
                proto::Phase phase = msg.phase_request();
                GameState::set_phase(phase);

                // Announce phase change to all clients.
                proto::ServerMessage phase_announce;
                phase_announce.set_phase_update(phase);
                server.send_to_all(phase_announce);
                break;
            }
            default:
                break;
            }
        }
    }
}

void ClientNetworkManager::connect() {
    Config::config->get_value(Config::str_server_ip, server_host);
    Config::config->get_value(Config::str_port, port);

    // Try establishing connection in separate thread.
    boost::thread(&ClientNetworkManager::attempt_connection, this).detach();
}

void ClientNetworkManager::register_listeners() {
    // Debug.
    events::debug::client_set_phase_event.connect([&](Phase* phase) {
        proto::ClientMessage msg;
        if (phase == &GameState::menu_phase)
            msg.set_phase_request(proto::Phase::MENU);
        else if (phase == &GameState::build_phase)
            msg.set_phase_request(proto::Phase::BUILD);
        else if (phase == &GameState::dungeon_phase)
            msg.set_phase_request(proto::Phase::DUNGEON);
        else if (phase == &GameState::minigame_phase)
            msg.set_phase_request(proto::Phase::MINIGAME);
        else {
            std::cerr << "Unrecognized phase. Use the static phases in GameState.\n";
            return;
        }
        client.send(msg);
    });

    // Build phase.
    events::build::request_build_event.connect([&](proto::Construct& c) {
        proto::ClientMessage msg;
        msg.set_allocated_build_request(new proto::Construct(c));
        client.send(msg);
    });

    // Dungeon phase.
    events::dungeon::network_player_move_event.connect([&](events::StickData d) {
        proto::ClientMessage msg;
        proto::StickData* pd = new proto::StickData();
        pd->set_input(
            d.input == events::Stick::STICK_LEFT ? 
            proto::StickData::STICK_LEFT :
            proto::StickData::STICK_RIGHT);
        pd->set_x(d.state.first);
        pd->set_y(d.state.second);
        pd->set_id(client_id);
        msg.set_allocated_move_request(pd);
        client.send(msg);
    });
}

void ClientNetworkManager::update() {
    proto::ServerMessage msg;
    while (client.read_message(&msg)) {
        switch (msg.message_type_case()) {
        case proto::ServerMessage::MessageTypeCase::kBuildUpdate: {
            proto::Construct construct = msg.build_update();
            if (construct.status())
                events::build::update_build_event(construct);
            else {
                // TODO: construct placement failed. client-side notification?
            }
            break;
        }
        case proto::ServerMessage::MessageTypeCase::kJoinResponse: {
            proto::JoinResponse resp = msg.join_response();
            if (resp.status()) {
                client_id = resp.id();
                player_id = GameState::add_existing_player(resp.obj_id())->get_id();
            }
            break;
        }
        case proto::ServerMessage::MessageTypeCase::kStateUpdate: {
            proto::GameState state = msg.state_update();
            bool all_exist = true;
            for (auto obj : state.objects()) {
                if (GameObject::game_objects.find(obj.id()) == GameObject::game_objects.end()) {
                    // Game object doesn't exist on this client yet; create.
                    std::cerr << "Creating obj with id " << obj.id() << "\n";
                    if (obj.type() == proto::GameObject::Type::GameObject_Type_PLAYER) {
                        events::menu::spawn_existing_player_event(obj.id());
                    }
                    else if (obj.type() == proto::GameObject::Type::GameObject_Type_GOAL) {
                        events::dungeon::spawn_existing_goal_event(obj.x(), obj.z(), obj.id());
                    }
                    else {
                        std::cerr << "Unrecognized obj type\n";
                    }
                    all_exist = false;
                }
                else {
                    if (obj.type() == proto::GameObject::Type::GameObject_Type_PLAYER) {
                        Player* player = dynamic_cast<Player*>(GameObject::game_objects[obj.id()]);
                        player->update_state(obj.x(), obj.z(), obj.wx(), obj.wz(), player_id == obj.id());
                    }
                    else {
                        GameObject::game_objects[obj.id()]->update_state(obj.x(), obj.z(), obj.wx(), obj.wz());
                    }
                }
            }
            if (all_exist) {
                for (int obj_id : state.collisions()) {
                    GameObject::game_objects[obj_id]->on_collision_graphical();
                }
            }
            break;
        }
        case proto::ServerMessage::MessageTypeCase::kPhaseUpdate: {
            proto::Phase phase = msg.phase_update();
            GameState::set_client_phase(phase);
            break;
        }
        default:
            break;
        }
    }
}

void ClientNetworkManager::attempt_connection() {
    bool first_attempt = true;
    double time_last_connect_attempt = glfwGetTime();

    // Try connecting to the host every 5 seconds.
    while (!is_connected) {
        if (first_attempt || glfwGetTime() - time_last_connect_attempt > CONN_RETRY_SEC) {
            first_attempt = false;
            std::cerr << "Attempting to connect to " << server_host << "...\n";
            is_connected = client.connect(server_host, port);
            time_last_connect_attempt = glfwGetTime();
            if (is_connected) {
                std::cerr << "Established connection.\n";
                register_listeners();
                service_thread = new boost::thread(&NetworkManager::run_service, this);
            }
        }
    }
}
