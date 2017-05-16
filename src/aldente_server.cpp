#include "aldente_server.h"

#include "util/config.h"
#include "game/game_state.h"
#include "net/network_manager.h"
#include "timer.h"
#include <chrono>
#include <iostream>
#include <boost/filesystem.hpp>

void AldenteServer::start() {
    Util::seed(0); // Seed PRNG.

    Timer timer(GAME_TICK);
    Timer::provide(&timer);

    // Game logic. Temporarily start game with build phase.
    GameState::setup(true);
    GameState::set_phase(&GameState::build_phase);

    ServerNetworkManager network;
    network.connect();

    std::cerr << "Starting server..." << std::endl;

    while (true) {
        network.update();

        Timer::get()->catch_up([&]{
            GameState::update();
        });
    }

    network.disconnect();
}
