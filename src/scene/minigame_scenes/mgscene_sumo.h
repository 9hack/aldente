#pragma once

#include "scene/scene.h"
#include "game_objects/minigame/ball.h"

/*
Scene for sumo minigame
*/
class MGSceneSumo : public Scene {
private:
    std::vector<Ball*> balls;
public:
    MGSceneSumo() {};
    void s_update() override;
    void c_update() override;
    void s_setup() override;
    void c_setup() override;
    void connect_listeners() override;
    void disconnect_listeners() override;
    void reset_camera() override;
    void reset_scene() override;
};