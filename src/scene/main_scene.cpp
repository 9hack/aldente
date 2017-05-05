#include "main_scene.h"
#include "game_objects/test_chest.h"
#include "game_objects/player.h"

MainScene::MainScene() : Scene() {
    light_pos = glm::vec3(0.f, 1.f, 1.f);

    //Setting up scene graph for Grid

    grid = new Grid(20, 20);
    std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
    for (int i = 0; i < toAdd.size(); i++) {
        std::vector<Tile *> currRow = toAdd[i];
        for (int j = 0; j < currRow.size(); j++) {
            /*Model *currTile = new Model();
            currTile->add_mesh(currRow[j]->getMesh());
            GameObject* currObj = new GameObject();
            currObj->attach_model(currTile);
            objs.push_back(currObj);*/
            objs.push_back(currRow[j]);

            if (currRow[j]->getRigid() != NULL) {
                //physics.dynamicsWorld->addRigidBody(currRow[j]->getRigid());
                //physics.rigidBodies.push_back(currRow[j]->getRigid());
                addRigid(currRow[j]->getRigid());
            }
        }
    }

    // Test Model loading
    Model *player_model = AssetLoader::get_model(std::string("boy_two"));
    Player *player = new Player();
    player_model->set_shader(&ShaderManager::anim_unlit);
    player->attach_model(player_model);
    player->start_walk();
    player->transform.set_scale({ 0.006f, 0.006f, 0.006f });
    player->transform.translate({ 4.f, 0.05f, 4.f });
    objs.push_back(player);

    Model *chestg_model = AssetLoader::get_model(std::string("chest_good"));
    TestChest *chestg = new TestChest();
    chestg_model->set_shader(&ShaderManager::anim_unlit);
    chestg->attach_model(chestg_model);
    chestg->open_chest();
    chestg->transform.set_scale({ 0.006f, 0.006f, 0.006f });
    chestg->transform.translate({ 5.f, 0.05f, 4.f });
    objs.push_back(chestg);

    Model *chestb_model = AssetLoader::get_model(std::string("chest_bad"));
    TestChest *chestb = new TestChest();
    chestb_model->set_shader(&ShaderManager::anim_unlit);
    chestb->attach_model(chestb_model);
    chestb->open_chest();
    chestb->transform.set_scale({ 0.006f, 0.006f, 0.006f });
    chestb->transform.translate({ 3.f, 0.05f, 4.f });
    objs.push_back(chestb);
}

void MainScene::update() {

    Scene::update();

    //hover = physics.hover; hover = nullptr;
    std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
    for (int i = 0; i < toAdd.size(); i++) {
        std::vector<Tile *> currRow = toAdd[i];
        for (int j = 0; j < currRow.size(); j++) {
            currRow[j]->update(hover);
        }
    }

    grid->update();
}