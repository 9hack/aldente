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

    std::vector<glm::vec3> player_start_pos{
        { -3, 0, -0.5f },
        { -2, 0, -0.5f },
        { 2, 0, -0.5f },
        { 3, 0, -0.5f },
    };
    //std::map<int, bool> dead_player_flags;
};