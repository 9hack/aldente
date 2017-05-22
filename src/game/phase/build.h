#pragma once

#include "phase.h"

const auto BUILD_TIME = std::chrono::seconds(10);

class BuildPhase : public TimedPhase {
public:
    BuildPhase(Context& context) : TimedPhase(context) {};
    void setup();
    void teardown();
    proto::Phase update();
    void client_setup();
    void client_teardown();
private:
    static bool is_menu;
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
    boost::signals2::connection ready_conn;
};
