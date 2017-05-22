#pragma once

#include "phase.h"

const auto BUILD_TIME = std::chrono::seconds(10);

class BuildPhase : public TimedPhase {
public:
    BuildPhase(Context& context) : TimedPhase(context) {};
    void setup();
    void teardown();
    void client_setup();
    void client_teardown();
private:
    static bool is_menu;
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
    boost::signals2::connection ready_conn;

    // <Player id, if ready>
    std::unordered_map<int, bool> ready_flags;
};
