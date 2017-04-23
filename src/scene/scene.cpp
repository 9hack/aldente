#include "scene.h"

#include "model/model.h"
#include "util/colors.h"
#include "asset_loader.h"

Scene::Scene(Physics &p) :
		physics(p) {
	root = new SceneGroup(this);
	camera = new SceneCamera(this);
}

Scene::~Scene() {
    delete (root);
    delete (camera);
}

void Scene::draw() {
    root->draw();
}

void Scene::update() {
    root->update();
}

void Scene::pass(Shader *s) {
    // For passing a different shader than the mesh's shader and drawing,
    // Used mainly for shadow mapping passes
    root->pass(s);
}

MainScene::MainScene(Physics &p) : Scene(p) {
	light_pos = glm::vec3(0.f, 2.f, 1.f);

	// Skybox
	Material *default_material = new Material();
	Mesh *skybox_mesh = new Mesh(nullptr, default_material, ShaderManager::get_shader_program("skybox"));
	Model *skybox_model = new Model(this);
	skybox_model->add_mesh(skybox_mesh);
	root->add_child(skybox_model);

	// Cube
	Geometry *cube_geo = GeometryGenerator::generate_cube(1.f, true);
	Material *cube_mat = new Material(color::ocean_blue);
	Mesh *cube_mesh = new Mesh(cube_geo, cube_mat);
	Model *cube_model = new Model(this);
	cube_model->model_mat *= glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 6.0f, 3.0f));
	cube_model->add_mesh(cube_mesh);
	root->add_child(cube_model);

	//Setting up scene graph for Grid

	grid = new Grid(10, 10);
	std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
	for (int i = 0; i < toAdd.size(); i++) {
		std::vector<Tile *> currRow = toAdd[i];
		for (int j = 0; j < currRow.size(); j++) {
			Model *currTile = new Model(this);
			currTile->add_mesh(currRow[j]->getMesh());
			root->add_child(currTile);

			/*
			if (currRow[j]->getRigid() != NULL) {
				physics.dynamicsWorld->addRigidBody(currRow[j]->getRigid());
				physics.rigidBodies.push_back(currRow[j]->getRigid());
			}*/

		}
	}

	// Test Model loading
	Model *tmodel = AssetLoader::asset_loader->getModel(std::string("textured.fbx"));
	tmodel->set_scene(this);
	root->add_child(tmodel);
}

void MainScene::update() {
	hover = physics.hover;

	std::vector<std::vector<Tile *>> toAdd = grid->getGrid();
	for (int i = 0; i < toAdd.size(); i++) {
		std::vector<Tile *> currRow = toAdd[i];
		for (int j = 0; j < currRow.size(); j++) {
			currRow[j]->update(hover);
		}
	}

	Scene::update();
}
