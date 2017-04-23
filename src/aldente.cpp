#include "aldente.h"

#include "window.h"
#include "asset_loader.h"
#include "physics.h"
#include "shadows.h"
#include "scene_manager.h"
#include "debug_input.h"
#include "poll/poller.h"
#include "poll/glfw_poller.h"
#include "poll/input_poller.h"
#include "util/config.h"
#include "events.h"

bool Aldente::shadows_on = true;
bool Aldente::debug_shadows = false;

Aldente::~Aldente() {
    ShaderManager::destroy();
    GeometryGenerator::clean_up();
    glfwTerminate();
}

static void glSetup() {
    assert(glewInit() == GLEW_OK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Aldente::start_game_loop() {
    Util::seed(0); // Seed PRNG.

    // Set up GLFW.
    assert(glfwInit());
    glfwSetErrorCallback([](int error, const char *description) {
        std::cerr << description << std::endl;
    });

    // Create a window.
    int width, height;
    Config::config->get_value(Config::str_screen_width, width);
    Config::config->get_value(Config::str_screen_height, height);
    std::string game_name;
    Config::config->get_value(Config::str_game_name, game_name);
    Window window(game_name, true, width, height);

    // Setup subsystems after window creation.
    glSetup();
    ShaderManager::init();
    AssetLoader::asset_loader->setup();

    // Set up list of polling objects.
    std::vector<std::shared_ptr<Poller>> pollers {
            std::make_shared<GlfwPoller>(),
            std::make_shared<InputPoller>(),
    };

	Physics physics;
    Shadows shadows(window);
    SceneManager scene_manager;

	// Init the test scene.
	MainScene *testScene = new MainScene();
	physics.set_scene(testScene);
	scene_manager.set_current_scene(testScene);
    DebugInput debug_input(window, scene_manager, physics);

    while (!window.should_close()) {
        // Do polling
        for (auto &poller : pollers) {
            poller->poll();
        }

        // Test fire for joystick events
        events::joystick_event.connect([](events::JoystickData &d) {
            fprintf(stderr,
                    "JoystickEvent:\n"
                            "  id: %d\n"
                            "  is_button: %d\n"
                            "  input: %d\n"
                            "  state: %d\n",
                    d.id, d.is_button, d.input, d.state);
        });

        debug_input.handle_movement();
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
