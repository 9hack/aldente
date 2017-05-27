#include "start_scene.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"

StartScene::StartScene() : Scene() {
}

void StartScene::s_update() {
    Scene::s_update();
}

void StartScene::c_update() {
    Scene::c_update();
}

void StartScene::s_setup() {    
}

void StartScene::c_setup() {
    // Set up platforms on which the player will stand.
    // Serves no functionality other than just being visible.
    for (int i = 0; i < 4; i++) {
        GameObject *platform = new GameObject();
        platform->transform.set_position(glm::vec3(2 * i, -0.8f, 0));
        platform->attach_model(AssetLoader::get_model("chest_good"));
        platform->transform.set_scale({ 0.006f, 0.006f, 0.006f });

        objs.push_back(platform);
        platforms.push_back(platform);
    }

    for (GameObject *obj : platforms) {
        obj->setup_model();
    }

    info.camera.cam_pos = glm::vec3(3.f, 0, 6.f);
    info.camera.recalculate();

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
        Color::WHITE, 0.5f);
    add_light(sun);
}

Player* StartScene::s_spawn_player(int conn_id) {
    Player *player = new Player();

    // TODO: determine where each player starts based on client id. 
    player->set_start_position({ (2 * (conn_id - 1)), 0, 0 });
    player->s_set_model_index(conn_id % Player::PLAYER_MODELS.size());
    player->reset_position();
    objs.push_back(player);

    return player;
}

Player* StartScene::c_spawn_player(int obj_id, int model_index) {
    Player *player = new Player(obj_id);
    player->c_setup_player_model(model_index);
    objs.push_back(player);

    return player;
}