#include "aldente.h"
#include "window.h"
#include "setup.h"
#include "asset_loader.h"
#include "physics.h"
#include "shadows.h"
#include "scene_manager.h"
#include "debug_input.h"
#include "poll/poller.h"
#include "poll/input_poller.h"
#include "util/config.h"

bool Aldente::shadows_on = true;
bool Aldente::debug_shadows = false;

Aldente::Aldente() {
    assert(glfwInit());
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << description << std::endl;
    });
    Setup::setup_callbacks();
}

Aldente::~Aldente() {
    ShaderManager::destroy();
    GeometryGenerator::clean_up();
    glfwTerminate();
}

void Aldente::start_game_loop() {

    // Make a window
    Window::set_hints();

    int width, height;
    Config::config->get_value(Config::str_screen_width, width);
    Config::config->get_value(Config::str_screen_height, height);
    std::string game_name;
    Config::config->get_value(Config::str_game_name, game_name);
    Window window(width, height, game_name);

    // Setup stuff
    Setup::setup_opengl();
    Setup::setup_shaders();
    AssetLoader::asset_loader->setup();
    Util::seed(0); // Seed PRNG.

    std::vector<Poller *> pollers {new InputPoller(window)};

	Physics physics;
    Shadows shadows(window);
    SceneManager scene_manager;

	//Init the test scene
	MainScene* testScene = new MainScene();
	physics.set_scene(testScene);
	scene_manager.set_current_scene(testScene);
    DebugInput debug_input(scene_manager,physics);


    while (!window.should_close()) {
        // Do polling
        glfwPollEvents();
        for (auto *poller : pollers) {
            poller->poll();
        }

        debug_input.handle_movement();

        window.update_size();
        physics.update();

        scene_manager.get_current_scene()->update();

        // First pass: shadowmap.
        shadows.shadow_pass(scene_manager.get_current_scene());

        // Second pass: usual rendering.
        window.clear();
        scene_manager.get_current_scene()->draw();

        if (debug_shadows)
            shadows.debug_shadows();

        window.swap_buffers();
    }
}
