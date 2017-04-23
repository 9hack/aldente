#include "scene.h"

#include "model/model.h"
#include "util/colors.h"
#include "asset_loader.h"
#include "scene_camera.h"

Scene::Scene() {
	camera = new SceneCamera();
}

Scene::~Scene() {
    delete (camera);
}

void Scene::draw() {
	for (GameObject* gameObj : objs) {
		gameObj->draw(camera);
	}
}

void Scene::update() {
	for (GameObject* gameObj : objs) {
		gameObj->update();
	}
}

void Scene::pass(Shader *s) {
    // For passing a different shader than the mesh's shader and drawing,
    // Used mainly for shadow mapping passes
	for (GameObject* gameObj : objs) {
		gameObj->pass(s);
	}
}

void Scene::addRigid(btRigidBody* toAdd) {
	rigids.push_back(toAdd);
	rigidSignal({ true, toAdd });
}

MainScene::MainScene() : Scene() {
	light_pos = glm::vec3(0.f, 2.f, 1.f);

	// Skybox
	Material *default_material = new Material();
	Mesh *skybox_mesh = new Mesh(nullptr, default_material, ShaderManager::get_shader_program("skybox"));
	Model *skybox_model = new Model();
	skybox_model->add_mesh(skybox_mesh);
	GameObject* sky = new GameObject();
	sky->attach_model(skybox_model);
	objs.push_back(sky);

	// Cube
	Geometry *cube_geo = GeometryGenerator::generate_cube(1.f, true);
	Material *cube_mat = new Material(color::ocean_blue);
	Mesh *cube_mesh = new Mesh(cube_geo, cube_mat);
	Model *cube_model = new Model();
	cube_model->model_mat *= glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 6.0f, 3.0f));
	cube_model->add_mesh(cube_mesh);
	GameObject* cube = new GameObject();
	cube->attach_model(cube_model);
	objs.push_back(cube);

	//Setting up scene graph for Grid

	grid = new Grid(10, 10);
	std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
	for (int i = 0; i < toAdd.size(); i++) {
		std::vector<Tile *> currRow = toAdd[i];
		for (int j = 0; j < currRow.size(); j++) {
			Model *currTile = new Model();
			currTile->add_mesh(currRow[j]->getMesh());
			GameObject* currObj = new GameObject();
			currObj->attach_model(currTile);
			objs.push_back(currObj);

			
			if (currRow[j]->getRigid() != NULL) {
				//physics.dynamicsWorld->addRigidBody(currRow[j]->getRigid());
				//physics.rigidBodies.push_back(currRow[j]->getRigid());
				addRigid(currRow[j]->getRigid());
			}

		}
	}

	// Test Model loading
	Model *tmodel = AssetLoader::asset_loader->getModel(std::string("textured.fbx"));
	GameObject *testObj = new GameObject();
	testObj->attach_model(tmodel);
	objs.push_back(testObj);
}

void MainScene::update() {
	//hover = physics.hover;

	std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
	for (int i = 0; i < toAdd.size(); i++) {
		std::vector<Tile *> currRow = toAdd[i];
		for (int j = 0; j < currRow.size(); j++) {
			currRow[j]->update(hover);
		}
	}

	Scene::update();
}
