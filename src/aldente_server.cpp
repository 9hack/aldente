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

    // Game logic. Temporarily start game with build phase.
    GameState::init(&GameState::build_phase);

    ServerNetworkManager network;
    network.connect();

    Timer timer(GAME_TICK);
    Timer::provide(&timer);

    Timer::get()->do_after(std::chrono::seconds(1), [](std::chrono::duration<double> d) {
        std::cout << "print me once after one second" << std::endl;
    });

    const std::function<void()> cancel = Timer::get()->do_every(
            std::chrono::milliseconds(500),
            [&cancel](std::chrono::duration<double> d) {
                static int count = 0;
                std::cout << "print me every 0.5 seconds: (" << ++count << " / 5)" << std::endl;
                if (count >= 5) cancel();
            });

    std::cerr << "Starting server..." << std::endl;

    while (true) {
        // std::cerr << "BEFORE: " << Timer::get()->so_far().count() << std::endl;
        network.update();
        // std::cerr << "NET: " << Timer::get()->so_far().count() << std::endl;
        GameState::update();
        // std::cerr << "GS: " << Timer::get()->so_far().count() << std::endl;
        Timer::get()->wait();
    }

    network.disconnect();
}
