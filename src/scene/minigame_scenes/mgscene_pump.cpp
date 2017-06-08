#include "mgscene_pump.h"
#include "game_objects/player.h"
#include "game_objects/minigame/platform.h"
#include "events.h"
#include "../../asset_loader.h"
#include "game_objects/minigame/pump.h"
#include "game_objects/minigame/hotair_basket.h"

void MGScenePump::s_update() {
    Scene::s_update();
}

void MGScenePump::c_update() {
    Scene::c_update();
}

void MGScenePump::s_setup() {
}

void MGScenePump::c_setup() {
    // Set up the platform
    Platform* platform = new Platform(PlatformShape::SQUARE);

    platform->set_position(glm::vec3(0, -0.5f, 0));
    platform->transform.set_scale(glm::vec3(10, 1.0f, 10));
    objs.push_back(platform);

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
        Color::WHITE, 0.5f);
    add_light(sun);

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
}

void MGScenePump::connect_listeners() {
}

void MGScenePump::disconnect_listeners() {
}

void MGScenePump::reset_camera() {
    info.camera.reset();
    info.camera.rotate_cam(glm::vec3(1, 0, 0), -45.0f);
    info.camera.cam_pos = glm::vec3(0, 8, 9);
    info.camera.recalculate();
}

void MGScenePump::reset_scene() {
}

void MGScenePump::c_add_pump(int to_set) {
    SumoBall *ball = new SumoBall(-1);
    ball->c_set_player(to_set);
    objs.push_back(ball);
    balls.push_back(ball);
}