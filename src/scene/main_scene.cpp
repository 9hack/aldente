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

    // Test Model loading
    Model *player_model = AssetLoader::get_model(std::string("boy_two_scaled"));
    Player *player = new Player();
    player_model->set_shader(&ShaderManager::anim_unlit);
    player->attach_model(player_model);
    player->transform.set_scale({ 0.4f, 0.4f, 0.4f });
    player->start_walk();
    player->transform.translate({ 2.f, 0.f, 2.f });
    objs.push_back(player);

    Model *chestg_model = AssetLoader::get_model(std::string("chest_good_scaled"));
    TestChest *chestg = new TestChest();
    chestg_model->set_shader(&ShaderManager::anim_unlit);
    chestg->attach_model(chestg_model);
    chestg->transform.set_scale({ 0.6f, 0.6f, 0.6f });
    chestg->open_chest();
    chestg->transform.translate({ 4.f, 0.05f, 1.f });
    objs.push_back(chestg);

    Model *chestb_model = AssetLoader::get_model(std::string("chest_bad_scaled"));
    TestChest *chestb = new TestChest();
    chestb_model->set_shader(&ShaderManager::anim_unlit);
    chestb->attach_model(chestb_model);
    chestb->transform.set_scale({ 0.6f, 0.6f, 0.6f });
    chestb->open_chest();
    chestb->transform.translate({ 29.f, 0.05f, 18.f });
    objs.push_back(chestb);
}