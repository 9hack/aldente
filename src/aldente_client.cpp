#include <input/modal_input.h>
#include "aldente_client.h"

#include "window.h"
#include "input/conceptual_translator.h"
#include "input/axis_combiner.h"
#include "input/raw_maps/xbox.h"
#include "input/raw_maps/matricom.h"
#include "input/raw_maps/debug.h"
#include "asset_loader.h"
#include "debug_input.h"
#include "poll/poller.h"
#include "poll/glfw_poller.h"
#include "poll/input_poller.h"
#include "util/config.h"
#include "events.h"
#include "timer.h"
#include "ui/ui_manager.h"
#include "render/render.h"
#include "game/game_state.h"
#include "net/network_manager.h"
#include "shaders/shader_manager.h"
#include "audio/audio_manager.h"

AldenteClient::~AldenteClient() {
    GeometryGenerator::destroy();
    glfwTerminate();
}

static void glSetup() {
    assert(glewInit() == GLEW_OK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void AldenteClient::start() {
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
    bool fullscreen;
    Config::config->get_value(Config::str_full_screen, fullscreen);
    Window window(game_name, true, width, height, fullscreen);
    std::tie(width, height) = window.get_size(); // get actual window dimensions after window creation

    // Create input handlers.
    string controller;
    Config::config->get_value(Config::str_controller, controller);
    const auto control_mapping =
            controller == "XBOX" ? input::BTN_MAP_XBOX :
            controller == "MATRICOM" ? input::BTN_MAP_MATRICOM :
            input::BTN_MAP_XBOX;
    input::ConceptualTranslator translator(control_mapping, input::KBD_MAP_DEBUG);
    input::AxisCombiner stick_handler(translator, input::STICKS_DEFAULT);
    input::ModalInput::provide(std::make_shared<input::ModalInput>());

    // Setup subsystems after window creation.
    glSetup();
    ShaderManager::init();
    AssetLoader::setup();

    // Audio
    AudioManager audio_manager;

    // Set up list of polling objects.
    std::vector<std::shared_ptr<Poller>> pollers {
        std::make_shared<GlfwPoller>(),
        std::make_shared<InputPoller>(),
    };

    // Used for callbacks
    Timer timer(GAME_TICK);
    Timer::provide(&timer);

    // Has logic based on phase change, so set ui up before GameState
    UIManager ui_manager((float) width / (float) height);

    // Game logic. Start game with menu phase.
    GameState::setup(false);
    GameState::set_phase(proto::Phase::MENU);

    Render render(window, GameState::scene_manager);

    DebugInput debug_input(window, GameState::scene_manager, GameState::physics);

    // Have window fire off a resize event to update all interested systems.
    window.broadcast_size();

    ClientNetworkManager network;
    network.connect();

    std::cerr << "Starting client..." << std::endl;

    // Setup framerate counter.
    GLuint frame = 0;
    Timer::get()->do_every(std::chrono::seconds(1), [&](){
        events::debug::fps_count_event(frame);
        frame = 0;
    });

    while (!window.should_close()) {
        frame++;

        // Do polling
        for (auto &poller : pollers) {
            poller->poll();
        }

        network.update();
        Timer::get()->catch_up();
        GameState::c_update();

        render.update();
        ui_manager.draw();
        window.swap_buffers();
    }

    network.disconnect();
}
