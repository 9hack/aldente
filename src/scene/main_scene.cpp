#include "main_scene.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"
#include "light/pulse_point_light.h"

#include "game_objects/essence.h" // TESTING ONLY
#include "util/util.h"

MainScene::MainScene() : Scene() {

}

void MainScene::s_update() {
    Scene::s_update();
}

void MainScene::c_update() {
    Scene::c_update();

    // Rotate directional light sources just to test shadows.
    if (lights_debug_on) {
        for (int i = 0; i < info.dir_lights.size(); ++i) {
            info.dir_lights[i]->transform.rotate(0.f, 0.f, 0.01f, false);
        }
    }
}

void MainScene::s_setup() {
    //Setting up map
    grid = new Grid("assets/maps/dungeon_wtf.txt");
    //grid = new Grid("assets/maps/dungeon_test.txt");
    objs.push_back(grid);
    
    // TESTING ONLY
    for (int i = 0; i < 40; i++) {
        Essence *ess = new Essence(1000 + i);
        ess->set_position({ 4.f + Util::random(-1.f, 1.f), 0.f, 4.f + Util::random(-1.f, 1.f) });
        objs.push_back(ess);
    }
}

void MainScene::c_setup() {
    //Setting up map
    grid = new Grid("assets/maps/dungeon_test.txt");
    objs.push_back(grid);

    for (int i = 0; i < 40; i++) {
        Essence *ess = new Essence(1000 + i);
        ess->set_position({ 4.f + Util::random(-1.f, 1.f), 0.f, 4.f + Util::random(-1.f, 1.f) });
        objs.push_back(ess);
    }

    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
                                                 Color::WHITE, 0.5f);
    add_light(sun);

    PointLight *bulb = new PointLight(glm::vec3(5.f, 2.f, 5.f), Color::BONE_WHITE, 0.1f);
    add_light(bulb);

    PulsePointLight *bulb2 = new PulsePointLight(glm::vec3(7.f, 2.f, 10.f), Color::WINDWAKER_GREEN);
    add_light(bulb2);

    SpotLight *spot_light = new SpotLight(glm::vec3(10.f, 4.f, 5.f),
                                          glm::vec3(0.f, -1.f, 0.f),
                                          Color::MAGENTA);
    add_light(spot_light);

    // Setup light debug callback.
    events::debug::toggle_light_rotation_event.connect([&](void) {
        lights_debug_on = !lights_debug_on;
    });

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
}

Player* MainScene::s_spawn_player(int conn_id) {
    Player *player = new Player();

    // TODO: determine where each player starts based on client id. 
    // For now, players 1-4 start at (2, 2), (2, 3), (2, 4), (2, 5) respectively.
    player->set_start_position({ 2.f, 0, 1.f + conn_id });
    player->reset_position();
    objs.push_back(player);

    return player;
}

Player* MainScene::c_spawn_player(int obj_id) {
    Player *player = new Player(obj_id);
    std::string name = "cat";
    player->setup_player_model(name);
    objs.push_back(player);

    return player;
}
