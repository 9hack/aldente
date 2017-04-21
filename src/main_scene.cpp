#include "main_scene.h"
#include "mesh.h"
#include "model\model.h"
#include "material.h"
#include "shaders\shader_manager.h"
#include "model\geometry_generator.h"
#include "grid.h"
#include "tile.h"
#include "physics.h"
#include "util\colors.h"
#include "assetLoader.h"

void MainScene::setup()
{
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
	cube_model->add_mesh(cube_mesh);
	root->add_child(cube_model);

	//Setting up scene graph for Grid

	grid = new Grid(10, 10);
	vector<vector<Tile*>> toAdd = grid->getGrid();
	for (int i = 0; i < toAdd.size(); i++)
	{
		vector<Tile*> currRow = toAdd[i];
		for (int j = 0; j < currRow.size(); j++)
		{
			Model *currTile = new Model(this);
			currTile->add_mesh(currRow[j]->getMesh());
			/*if (currRow[j]->getRigid() == NULL) {
			SceneTransform *tileTranslate = new SceneTransform(scene,
			glm::translate(glm::mat4(1.f), glm::vec3(currRow[j]->getX(), 0, currRow[j]->getZ())));
			tileTranslate->add_child(currTile);
			scene->root->add_child(tileTranslate);
			}
			else {*/
			root->add_child(currTile);
			//}

			if (currRow[j]->getRigid() != NULL)
			{
				Physics::dynamicsWorld->addRigidBody(currRow[j]->getRigid());
				Physics::rigidBodies.push_back(currRow[j]->getRigid());
			}

		}
	}

	// Test Model loading
	Model* tmodel = AssetLoader::getModel(std::string("textured.fbx"));
	tmodel->set_scene(this);
	root->add_child(tmodel);

	/*
	// Plane
	Geometry *plane_geo = GeometryGenerator::generate_plane(50.f, 0);
	Material plane_mat;
	plane_mat.diffuse = plane_mat.ambient = color::indian_red;
	Mesh plane_mesh = { plane_geo, plane_mat, ShaderManager::get_default(), glm::mat4(1.f) };
	SceneModel *plane_model = new SceneModel(scene);
	plane_model->add_mesh(plane_mesh);
	SceneTransform *plane_scale = new SceneTransform(scene, glm::scale(glm::mat4(1.f), glm::vec3(50.f, 1.0f, 50.f)));
	SceneTransform *plane_translate = new SceneTransform(scene, glm::translate(glm::mat4(1.f), glm::vec3(0.0f, -0.5f * PLAYER_HEIGHT, 0.0f)));
	plane_scale->add_child(plane_model);
	plane_translate->add_child(plane_scale);
	scene->root->add_child(plane_translate);
	*/
}

void MainScene::update()
{
	hover = Physics::hover;

	vector<vector<Tile*>> toAdd = grid->getGrid();
	for (int i = 0; i < toAdd.size(); i++)
	{
		vector<Tile*> currRow = toAdd[i];
		for (int j = 0; j < currRow.size(); j++)
		{
			currRow[j]->update(hover);
		}
	}

	Scene::update();
}