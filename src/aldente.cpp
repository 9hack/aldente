#include "aldente.h"

#include "window.h"
#include "keyboard.h"
#include "shaders/skybox_shader.h"
#include "shaders/shadow_shader.h"
#include "model/geometry_generator.h"
#include "model/model.h"

#include "grid.h"
#include "tile.h"

#include "events/input.h"
#include "input/process.h"
#include "util/util.h"
#include "util/colors.h"
#include "util/config.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cfloat>
#include <string>

namespace input = kuuhaku::input;
namespace events = kuuhaku::events;

bool Aldente::debug_shadows = false;
bool Aldente::shadows_on = true;

Grid* grid;
Tile* hover;

void Aldente::destroy()
{
    // Free memory here.
    ShaderManager::destroy();
    GeometryGenerator::clean_up();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Aldente::setup_scenes()
{
    // Initial scene and cam.
    scene = new Scene();
    scenes.push_back(scene);
    camera = scene->camera;
    scene->light_pos = glm::vec3(0.f, 2.f, 1.f);

    // Skybox
    Material default_material;
    Mesh *skybox_mesh = new Mesh{ nullptr, default_material, ShaderManager::get_shader_program("skybox"), glm::mat4(1.f) };
    Model *skybox_model = new Model(scene);
    skybox_model->add_mesh(skybox_mesh);
    scene->root->add_child(skybox_model);

    // Cube
    Geometry *cube_geo = GeometryGenerator::generate_cube(1.f, true);
    Material cube_mat;
    cube_mat.diffuse = cube_mat.ambient = color::ocean_blue;
    Mesh* cube_mesh = new Mesh{ cube_geo, cube_mat, ShaderManager::get_default(), glm::mat4(1.f) };
    SceneModel *cube_model = new SceneModel(scene);
    cube_model->add_mesh(cube_mesh);
    scene->root->add_child(cube_model);

	//Setting up scene graph for Grid

	grid = new Grid(10,10);
	vector<vector<Tile*>> toAdd = grid->getGrid();
	for (int i = 0; i < toAdd.size(); i++) {
		vector<Tile*> currRow = toAdd[i];
		for (int j = 0; j < currRow.size(); j++) {
			SceneModel *currTile = new SceneModel(scene);
			currTile->add_mesh(currRow[j]->getMesh());
			/*if (currRow[j]->getRigid() == NULL) {
				SceneTransform *tileTranslate = new SceneTransform(scene,
				  glm::translate(glm::mat4(1.f), glm::vec3(currRow[j]->getX(), 0, currRow[j]->getZ())));
				tileTranslate->add_child(currTile);
				scene->root->add_child(tileTranslate);
			}
			else {*/
				scene->root->add_child(currTile);
			//}

			if (currRow[j]->getRigid() != NULL) {
				dynamicsWorld->addRigidBody(currRow[j]->getRigid());
				rigidBodies.push_back(currRow[j]->getRigid());
			}

		}
	}

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

void Aldente::go()
{	
    // Get window width and height from config
    int width, height;
    Config::config->get_value(Config::str_screen_width, width);
    Config::config->get_value(Config::str_screen_height, height);
    std::string game_name;
    Config::config->get_value(Config::str_game_name, game_name);

    window = Window::create_window(width, height, game_name.c_str());
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Don't show cursor
	Setup::setup_callbacks();
    Setup::setup_opengl();

    Setup::setup_shaders();	

	// Seed PRNG.
    Util::seed(0);
    setup_scenes();

    // Send height/width of window
    glfwGetFramebufferSize(window, &width, &height);
    resize_callback(window, width, height);

    GLuint frame = 0;
    double prev_ticks = glfwGetTime();
    double move_prev_ticks = prev_ticks;

	AssetLoader* test = new AssetLoader();
	SceneModel* tmodel = test->getModel(std::string("textured.fbx"));
	tmodel->setScene(scene);
	scene->root->add_child(tmodel);
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        input::process();

		// FPS Control
        frame++;
        double curr_time = glfwGetTime();
        if (curr_time - prev_ticks > 1.f)
        {
            /* std::cerr << "FPS: " << frame << std::endl; */
            frame = 0;
            prev_ticks = curr_time;
        }
        if (curr_time - move_prev_ticks > 1.f / 60.f)
        {
            handle_movement();
            move_prev_ticks = curr_time;
        }		

		Physics::update();

		vector<vector<Tile*>> toAdd = grid->getGrid();
		for (int i = 0; i < toAdd.size(); i++) {
			vector<Tile*> currRow = toAdd[i];
			for (int j = 0; j < currRow.size(); j++) {
				currRow[j]->update(hover);
			}
		}

		scene->update();

		// Updates Window and Camera View Frustrum
        glfwGetFramebufferSize(window, &width, &height);
        scene->camera->update_frustum_planes();
        scene->camera->update_frustum_corners(width, height, FAR_PLANE);

        // First pass: shadowmap.
        shadow_pass();

        // Second pass: usual rendering.
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene->draw();

        // Debug shadows.
        if (debug_shadows)
        {     
			Shadows::debug_shadows();
        }

        glfwSwapBuffers(window);
    }
    destroy();
}

std::vector<Scene*> Aldente::get_scenes()
{
	return scenes;
}

Scene *Aldente::get_scene()
{
	return scene;
}

SceneCamera *Aldente::get_camera()
{
	return camera;
}

GLFWwindow *Aldente::get_window()
{
	return window;
}

