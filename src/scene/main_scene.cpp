#include "main_scene.h"
#include "game_objects/player.h"
#include "events.h"
#include "util/color.h"
#include "light/pulse_point_light.h"

MainScene::MainScene() : Scene(), goal(nullptr) {
    events::dungeon::s_prepare_dungeon_event.connect([&]() {
        remove_goal();
        s_place_goal(glm::vec3(0.0f), 20);
    });

    events::dungeon::spawn_existing_goal_event.connect([&](int x, int z, int id) {
        std::unique_lock<std::mutex> lock(goal_mutex);
        if (goal) {
            auto position = std::find(objs.begin(), objs.end(), goal);
            if (position != objs.end())
                objs.erase(position);
            goal = nullptr;
        }
        c_place_goal(x, z, id);
    });
}

void MainScene::s_update() {
    Scene::s_update();
}

void MainScene::c_update() {
    Scene::c_update();

    // Rotate directional light sources just to test shadows.
    if (lights_debug_on) {
        for (int i = 0; i < info.dir_lights.size(); ++i) {
            info.dir_lights[i]->transform.rotate(0.f, 0.f, 0.01f, false);
        }
    }
}

void MainScene::setup_scene() {
    //Setting up map
    grid = new Grid("assets/maps/dungeon_wtf.txt");
    //grid = new Grid("assets/maps/dungeon_test.txt");
    objs.push_back(grid);

}

void MainScene::graphical_setup() {
    // Setup lights.
    DirectionalLight *sun = new DirectionalLight(glm::vec3(0.f, -1.f, -1.f),
                                                 Color::WHITE, 0.5f);
    add_light(sun);

    PointLight *bulb = new PointLight(glm::vec3(5.f, 2.f, 5.f), Color::BONE_WHITE, 0.1f);
    add_light(bulb);

    PulsePointLight *bulb2 = new PulsePointLight(glm::vec3(7.f, 2.f, 10.f), Color::WINDWAKER_GREEN);
    add_light(bulb2);

    SpotLight *spot_light = new SpotLight(glm::vec3(10.f, 4.f, 5.f),
                                          glm::vec3(0.f, -1.f, 0.f),
                                          Color::MAGENTA);
    add_light(spot_light);

    // Setup light debug callback.
    events::debug::toggle_light_rotation_event.connect([&](void) {
        lights_debug_on = !lights_debug_on;
    });

    for (GameObject *obj : objs) {
        obj->setup_model();
    }
}

Player* MainScene::s_spawn_player(int conn_id) {
    Player *player = new Player();

    // TODO: determine where each player starts based on client id. 
    // For now, players 1-4 start at (2, 2), (2, 3), (2, 4), (2, 5) respectively.
    player->set_start_position({ 2.f, 0, 1.f + conn_id });
    player->reset_position();
    objs.push_back(player);

    return player;
}

Player* MainScene::c_spawn_player(int obj_id) {
    Player *player = new Player(obj_id);
    std::string name = "cat";
    player->setup_player_model(name);
    objs.push_back(player);

    return player;
}

void MainScene::s_place_goal(glm::vec3 start, int min_dist) {
    // Goal will be in range of (min_dist, edge of map)
    int new_goal_x = rand() % grid->get_width();
    int new_goal_z = rand() % grid->get_height();

    // If not buildable or too close, find another
    while (!grid->get_grid()[new_goal_z][new_goal_x]->isBuildable() ||
        (abs(new_goal_x - start.x) + abs(new_goal_z - start.z) < min_dist)) {
        new_goal_x = rand() % grid->get_width();
        new_goal_z = rand() % grid->get_height();
    }

    Goal *new_goal = new Goal(new_goal_x, new_goal_z);

    grid->get_grid()[new_goal_z][new_goal_x]->set_construct(new_goal);
    goal = new_goal;
    goal_x = new_goal_x;
    goal_z = new_goal_z;
}

void MainScene::c_place_goal(int x, int z, int id) {
    goal = new Goal(x, z, id);

    goal->setup_model();
    objs.push_back(goal);
}

void MainScene::remove_goal() {
    //TODO destructor for goal
    if (goal) {
        GameObject::game_objects.erase(goal->get_id());
        grid->get_grid()[goal_z][goal_x]->set_construct(nullptr);
        events::remove_rigidbody_event(goal);
    }
}