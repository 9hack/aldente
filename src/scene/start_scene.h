#pragma once

#include "scene/scene.h"
#include "asset_loader.h"
#include "game_objects/grid.h"
#include "game_objects/player.h"
#include "light/pulse_point_light.h"

/*
Starting scene to be used before the gameplay. Will hold things
like the logo screen and character selection.
*/
class StartScene : public Scene {
private:
    std::vector<GameObject*> platforms;
public:
    StartScene();
    void s_update() override;
    void c_update() override;
    void s_setup() override;
    void c_setup() override;

    Player* s_spawn_player(int conn_id); // Server: spawn new Player, auto-assign id.
    Player* c_spawn_player(int obj_id, int model_index); // Client: spawn existing Player with id from server.
};