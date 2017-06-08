#pragma once

#include "scene/scene.h"

/*
Scene for pump minigame
*/
class MGScenePump : public Scene {
private:
    std::vector<Pump*> pumps;
public:
    MGScenePump() {};
    void s_update() override;
    void c_update() override;
    void s_setup() override;
    void c_setup() override;
    void connect_listeners() override;
    void disconnect_listeners() override;
    void reset_camera() override;
    void reset_scene() override;
    void c_add_pump();
};