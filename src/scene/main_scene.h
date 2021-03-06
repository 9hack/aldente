#pragma once

#include "scene/scene.h"
#include "asset_loader.h"
#include "game_objects/grid.h"
#include "game_objects/player.h"
#include "light/pulse_point_light.h"

/*
Main Scene for having stuff in our game, especially for testing.
Anything that you would want to add to the game scene that is only
used for the specific scene should be put here.
In the future, the "Dungeon/Build Phase" would be its own scene, and
then each mini game would likely all be its own scene. The title menu
can also be its own scene.
*/
class MainScene : public Scene {
private:
    Grid *grid;
    Tile *hover;
    bool lights_debug_on = false;

    Goal *goal;
    PulsePointLight *goal_light;
    int goal_z, goal_x;

    boost::signals2::connection build_conn;
    boost::signals2::connection dungeon_conn;
    boost::signals2::connection delayed_goal_conn;
    boost::signals2::connection goal_conn;

    void add_random_lights();
    static std::vector<Color> light_colors;
public:
    MainScene();
    void s_update() override;
    void c_update() override;
    void s_setup() override;
    void c_setup() override;
    void connect_listeners() override;
    void disconnect_listeners() override;

    void s_place_goal(glm::vec3 start, int min_dist);
    void c_place_goal(int x, int z, int id);
    void remove_goal();

    void disable_scene() override;
    void enable_scene() override;
};