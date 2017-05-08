#include "network_manager.h"
#include "util/config.h"
#include "game_objects/player.h"
#include "game/game_state.h"
#include <GLFW/glfw3.h>

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
        }
        catch (std::exception & ex) {
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
    events::new_connection_event.connect([&](int conn_id) {
        events::menu::request_join_event(conn_id);
    });

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
    events::dungeon::network_positions_event.connect([&](std::map<int, Player*> & players) {
        for (auto & pair : players) {
            // TODO this assumes only one player connected
            proto::ServerMessage msg;
            proto::Player* p = new proto::Player();
            p->set_id(pair.first);
            p->set_x(pair.second->transform.get_position().x);
            p->set_z(pair.second->transform.get_position().z);
            p->set_wx(pair.second->direction.x);
            p->set_wz(pair.second->direction.z);
            msg.set_allocated_move_update(p);
            server.send_to_all(msg);
        }
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
                events::StickData d;
                d.input = stick.input() == proto::StickData_Stick::StickData_Stick_STICK_LEFT ?
                    events::Stick::STICK_LEFT :
                    events::Stick::STICK_RIGHT;
                d.state.first = stick.x();
                d.state.second = stick.y();
                events::dungeon::player_move_event(stick.id(), d);
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
        if (phase == &GameState::menu_phase) {

        }
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
            proto::StickData_Stick::StickData_Stick_STICK_LEFT :
            proto::StickData_Stick::StickData_Stick_STICK_RIGHT);
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
        }
        case proto::ServerMessage::MessageTypeCase::kJoinResponse: {
            proto::JoinResponse resp = msg.join_response();
            if (resp.status()) {
                proto::Player p;
                client_id = resp.id();
                p.set_id(client_id);
                events::menu::spawn_player_event(p);
            }
        }
        case proto::ServerMessage::MessageTypeCase::kMoveUpdate: {
            proto::Player p = msg.move_update();
            events::dungeon::set_player_pos_event(p.id(), p.x(), p.z(), p.wx(), p.wz());
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
