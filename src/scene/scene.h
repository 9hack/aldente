#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "scene_camera.h"
#include "scene_group.h"
#include "shaders/shader.h"
#include "model/plane.h"
#include "grid.h"
#include "physics.h"

/*
	Contains all the information for a single scene
*/
class Scene {
public:
    SceneGroup *root;
    SceneCamera *camera;
    glm::vec3 light_pos;
	Physics &physics;

    Scene(Physics &p);

    virtual ~Scene();

    void draw();

    void pass(Shader *s);

    virtual void update();
};

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
public:
	MainScene(Physics &p);
	void update() override;
};
