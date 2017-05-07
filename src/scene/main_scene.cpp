#include "main_scene.h"
#include "game_objects/test_chest.h"
#include "game_objects/player.h"
#include "events.h"

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

    // Rotate light source just to test shadows.
    if (lights_debug_on)
        light_pos = glm::vec3(glm::rotate(glm::mat4(1.f), 0.01f, glm::vec3(0.f, 0.f, 1.f)) * glm::vec4(light_pos, 1.f));
}

void MainScene::setup_scene() {
    // Setup light debug callback.
    events::debug::toggle_light_rotation_event.connect([&](void) {
        lights_debug_on = !lights_debug_on;
    });

    light_pos = glm::vec3(0.f, 2.f, 1.f);

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