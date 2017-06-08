#pragma once

#include "scene/scene.h"
#include "minigame.h"

/*
Holds the functionality for the "Hot Air Balloon" minigame
*/

const auto PUMPMG_TIME = std::chrono::seconds(60);
const int PUMPMG_REWARD = 100;

class PumpMG : public Minigame {
public:
    PumpMG(Context& to_set);

    void s_setup() override;
    void s_teardown() override;
    void c_setup() override;
    void c_teardown() override;

    bool is_finished() override;
private:
    boost::signals2::connection button_conn;

    //std::map<int, bool> dead_player_flags;

};