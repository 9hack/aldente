#include "main_scene.h"
#include "game_objects/test_chest.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"

MainScene::MainScene() : Scene() {

}

void MainScene::update() {

    Scene::update();

    //hover = physics.hover; hover = nullptr;
    std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
    for (int i = 0; i < toAdd.size(); i++) {
        std::vector<Tile *> currRow = toAdd[i];
        for (int j = 0; j < currRow.size(); j++) {
            currRow[j]->update();
        }
    }

    grid->update();

    // Rotate directional light sources just to test shadows.
    if (lights_debug_on) {
        for (int i = 0; i < info.dir_lights.size(); ++i) {
            info.dir_lights[i].transform.rotate(0.f, 0.f, 0.01f, false);
        }
    }
}

void MainScene::setup_scene() {

    // Setup lights.
    DirectionalLight sun = DirectionalLight(glm::vec3(0.f, -1.f, -1.f));
    add_light(sun);

    PointLight bulb = PointLight(glm::vec3(5.f, 2.f, 5.f), Color::BONE_WHITE);
    add_light(bulb);

    PointLight bulb2 = PointLight(glm::vec3(7.f, 2.f, 10.f), Color::WINDWAKER_GREEN);
    add_light(bulb2);

    SpotLight spot_light = SpotLight(glm::vec3(10.f, 4.f, 5.f),
                                     glm::vec3(0.f, -1.f, 0.f),
                                     Color::MAGENTA);
    add_light(spot_light);

    // Setup light debug callback.
    events::debug::toggle_light_rotation_event.connect([&](void) {
        lights_debug_on = !lights_debug_on;
    });

    //Setting up scene graph for Grid
    grid = new Grid("assets/maps/dungeon_test.txt");
    //grid = new Grid(20, 20);
    std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
    for (int i = 0; i < toAdd.size(); i++) {
        std::vector<Tile *> currRow = toAdd[i];
        for (int j = 0; j < currRow.size(); j++) {
            objs.push_back(currRow[j]);
        }
    }
}

void MainScene::graphical_setup_scene() {
    grid->graphical_setup();

    // Player instantiation will be here for now until we start working on
    // scene management.
    Model *player_model = AssetLoader::get_model(std::string("boy_two_scaled"));
    Player *player = new Player();
    player->transform.set_scale({ 0.4f, 0.4f, 0.4f });
    player->transform.translate({ 2.f, 0.f, 2.f });
    objs.push_back(player);
    player_model->set_shader(&ShaderManager::anim_unlit);
    player->attach_model(player_model);
    player->start_walk();

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
}