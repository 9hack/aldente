#include "scene.h"

#include "model/model.h"
#include "util/colors.h"
#include "asset_loader.h"
#include "scene_camera.h"
#include "scene_info.h"

void Scene::draw(Shader *shader) {
    SceneInfo scene_info = { &camera, light_pos };
    for (GameObject *gameObj : objs) {
        gameObj->draw(shader, scene_info);
    }
}

void Scene::draw_skybox() {
    SceneInfo scene_info = { &camera, light_pos };
    ShaderManager::skybox.use();
    ShaderManager::skybox.draw(NULL, scene_info); // NULL because no mesh needed.
}

void Scene::update() {
    for (GameObject *gameObj : objs) {
        gameObj->update();
    }
}

void Scene::addRigid(btRigidBody* toAdd) {
    rigids.push_back(toAdd);
    rigidSignal({ true, toAdd });
}

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
    Model *tmodel = AssetLoader::get_model(std::string("chest_good"));
    GameObject *testObj = new GameObject();
    tmodel->set_shader(&ShaderManager::anim_unlit);
    testObj->attach_model(tmodel);
    testObj->transform.set_scale({0.008f, 0.008f, 0.008f});
    testObj->transform.translate({0.f, 1.f, 0.f});
    objs.push_back(testObj);
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
