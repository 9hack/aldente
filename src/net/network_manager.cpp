#include "network_manager.h"
#include "util/config.h"
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
        } catch (...) {
            // Silently ignore errors rather than crash. Shouldn't happen.
            std::cerr << "run_service: io_service error\n";
        }
    }
}

void ServerNetworkManager::connect() {
    register_listeners();
    service_thread = new boost::thread(&NetworkManager::run_service, this);
}

void ServerNetworkManager::register_listeners() {
    // Menu phase.
    events::menu::respond_join_event.connect([&](proto::JoinResponse& resp) {
        proto::ServerMessage msg;
        msg.set_allocated_join_response(new proto::JoinResponse(resp));
        server.send_to_all(msg);
    });

    // Build phase.
    events::build::respond_build_event.connect([&](proto::Construct& c) {
        proto::ServerMessage msg;
        msg.set_allocated_build_update(new proto::Construct(c));
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
            case proto::ClientMessage::MessageTypeCase::kJoinRequest: {
                std::string player_name = msg.join_request();
                events::menu::request_join_event(player_name);
                std::cerr << "[s] got join request...\n";
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
    // Build phase.
    events::build::request_build_event.connect([&](proto::Construct& c) {
        proto::ClientMessage msg;
        msg.set_allocated_build_request(new proto::Construct(c));
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
                std::cerr << "[c] spawning player...\n";
                proto::Player p;
                events::menu::spawn_player_event(p);

                // My ID is num_players
            }
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
                
                // Request server to join the game.
                proto::ClientMessage join_msg;
                join_msg.set_join_request("Ethan Chan");
                client.send(join_msg);
            }
        }
    }
}
