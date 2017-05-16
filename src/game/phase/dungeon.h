#pragma once

#include "phase.h"

const auto DUNGEON_TIME = std::chrono::seconds(10);

class DungeonPhase : public TimedPhase {
public:
    DungeonPhase(Context& context) : TimedPhase(context) {};
    void setup();
    Phase* update();
    void teardown();
    void client_setup();
    void client_update();
    void client_teardown();
private:
    boost::signals2::connection joystick_conn;
    boost::signals2::connection button_conn;
    boost::signals2::connection collision_conn;
    boost::signals2::connection interact_conn;
};
