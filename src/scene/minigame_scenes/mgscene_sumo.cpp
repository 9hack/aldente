#include <timer.h>
#include "mgscene_sumo.h"
#include "game_objects/player.h"
#include "game_objects/minigame/empty_collider.h"
#include "game_objects/minigame/platform.h"
#include "events.h"
#include "util/color.h"
#include "../../asset_loader.h"

void MGSceneSumo::s_update() {
    Scene::s_update();
}

void MGSceneSumo::c_update() {
    Scene::c_update();
}

void MGSceneSumo::s_setup() {
    // Set up the platform
    Platform* platform = new Platform(PlatformShape::CIRCLE);

    platform->set_position(glm::vec3(0, -1.0f, 0));
    objs.push_back(platform);

    // Set up the collider below the platform to detect things falling off.
    EmptyCollider* collider = new EmptyCollider(-1);
    // Set position
    collider->set_position(glm::vec3(0, -8, 0));
    objs.push_back(collider);

    /*// Instantiate balls for players
    for (unsigned int i = 0; i < 4; i++) {
        Ball *ball = new Ball();
        objs.push_back(ball);
        balls.push_back(ball);
    }*/
}

void MGSceneSumo::c_setup() {
    // Set up the platform
    Platform* platform = new Platform(PlatformShape::CIRCLE);

    platform->set_position(glm::vec3(0, 0.2f, 0));
    platform->transform.set_scale(glm::vec3(6, 0.5f, 6));
    objs.push_back(platform);

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
        Color::WHITE, 0.5f);
    add_light(sun);

    /*// Instantiate balls for players
    for (unsigned int i = 0; i < 4; i++) {
        Ball *ball = new Ball();
        objs.push_back(ball);
        balls.push_back(ball);
    }*/

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
}

void MGSceneSumo::connect_listeners() {
}

void MGSceneSumo::disconnect_listeners() {
}

void MGSceneSumo::reset_camera() {
    info.camera.reset();
    info.camera.rotate_cam(glm::vec3(1, 0, 0), -65.0f);
    info.camera.cam_pos = glm::vec3(0, 6, 5);
    info.camera.recalculate();
}

void MGSceneSumo::reset_scene() {
}

void MGSceneSumo::c_add_ball(Player *to_set) {
    Ball *ball = new Ball(-1);
    ball->c_set_player(to_set);
    objs.push_back(ball);
    balls.push_back(ball);
}