#pragma once

#include "scene/scene_camera.h"
#include "model/model.h"

/*
	This represents an abstract object in the scene.
	Contains all the essential components required to
	render the model in the scene.
*/
class GameObject {
protected:
    Model *model;
	glm::mat4 to_world;
public:
    GameObject();

    void attach_model(Model *m);

    void draw(SceneCamera &camera);

	void update();

    void pass(Shader *s);

	glm::vec3 getPosition();

	glm::vec3 setPosition(glm::vec3 toSet);


};

