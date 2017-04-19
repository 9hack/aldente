#include "aldente.h"

#include "window.h"
#include "keyboard.h"
#include "shaders/skybox_shader.h"
#include "shaders/shadow_shader.h"
#include "geometry/geometry_generator.h"
#include "scene/model.h"

#include "events/input.h"
#include "input/process.h"
#include "global.h"
#include "util/util.h"
#include "util/colors.h"
#include "util/config.h"
#include "btBulletDynamicsCommon.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cfloat>
#include <string>

namespace input = kuuhaku::input;
namespace events = kuuhaku::events;

/* global vars */
Scene* scene;
std::vector<Scene*> scenes;
SceneCamera* camera;

bool debug_shadows = false;
bool mouse_moved = false;
bool shadows_on = true;
bool god_mode = false;
bool helicopter_mode = false;
glm::vec3 last_cursor_pos;

const GLfloat FAR_PLANE = 100.0f;
const GLfloat FOV = 45.f;
const GLfloat BASE_CAM_SPEED = 0.1f;
const GLfloat EDGE_PAN_THRESH = 5.f;
const GLfloat EDGE_PAN_SPEED = 0.5f;

//Bullet varaibles
btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;
vector<btRigidBody*> rigidBodies;

Grid* grid;
Tile* hover;

Aldente::Aldente() {}
Aldente::~Aldente() {}

void Aldente::destroy()
{
    // Free memory here.
    ShaderManager::destroy();
    GeometryGenerator::clean_up();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Aldente::setup_shaders()
{
    // Load shaders via a shader manager.
    ShaderManager::create_shader_program("basic");
    ShaderManager::create_shader_program("skybox");
    ShaderManager::create_shader_program("shadow");
    ShaderManager::create_shader_program("debug_shadow");
    ShaderManager::set_default("basic");
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
    setup_callbacks();
    setup_opengl();

    setup_shaders();

	// Initialize Bullet. This strictly follows http://bulletphysics.org/mediawiki-1.5.8/index.php/Hello_World,
	// even though we won't use most of this stuff.

	// Build the broadphase
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

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

		//Step in simulation
		dynamicsWorld->stepSimulation(1.f / 60.f, 10);

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
            glViewport(0, 0, width / 3, height / 3);
            ShaderManager::get_shader_program("debug_shadow")->use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ((ShadowShader *)ShaderManager::get_shader_program("shadow"))->shadow_map_tex);
            Util::render_quad();
        }

        glfwSwapBuffers(window);
    }
    destroy();
}

void Aldente::shadow_pass()
{
    ShadowShader * ss = (ShadowShader *) ShaderManager::get_shader_program("shadow");
    glViewport(0, 0, ss->size, ss->size);
    glBindFramebuffer(GL_FRAMEBUFFER, ss->FBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    ss->use();
    ss->light_pos = scene->light_pos;
    if (shadows_on) {
        ss->light_proj = scene->camera->frustum_ortho();
    }
    else {
        ss->light_proj = glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.f, 0.1f);
    }
    // Render using scene graph.
    glDisable(GL_CULL_FACE);
    scene->pass(ss);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Aldente::handle_movement()
{
    GLfloat cam_step = keys[GLFW_KEY_LEFT_SHIFT] ? 3*BASE_CAM_SPEED : BASE_CAM_SPEED;
    glm::vec3 displacement(0.f);

    int btn_c;
    const unsigned char* btns =
        glfwGetJoystickButtons(GLFW_JOYSTICK_1, &btn_c);

    if (keys[GLFW_KEY_W] || (btns && btns[13]))
        displacement += cam_step * camera->cam_front;
    if (keys[GLFW_KEY_S] || (btns && btns[15]))
        displacement -= cam_step * camera->cam_front;
    if (keys[GLFW_KEY_A] || (btns && btns[16]))
        displacement -= glm::normalize(glm::cross(camera->cam_front, camera->cam_up)) * cam_step;
    if (keys[GLFW_KEY_D] || (btns && btns[14]))
        displacement += glm::normalize(glm::cross(camera->cam_front, camera->cam_up)) * cam_step;
    if (keys[GLFW_KEY_SPACE])
        displacement += cam_step * camera->cam_up;

    camera->cam_pos += displacement;
    camera->recalculate();
}

void Aldente::setup_callbacks()
{
    glfwSetErrorCallback(Window::error_callback);
    glfwSetKeyCallback(window, Keyboard::key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    events::JoystickEvent::subscribe([](events::JoystickData d) {
        fprintf(stderr,
                "JoystickEvent:\n"
                "  id: %d\n"
                "  is_button: %d\n"
                "  input: %d\n"
                "  state: %d\n",
                d.id, d.is_button, d.input, d.state);
    });
}

void Aldente::setup_opengl()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        glfwTerminate();
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}
