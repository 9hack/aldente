#pragma once

#include "scene/scene_camera.h"
#include "model/model.h"
#include "transform.h"

/*
	This represents an abstract object in the scene.
	Contains all the essential components required to
	render the model in the scene.
*/
class GameObject {
protected:
    Model *model;
	//glm::mat4 to_world; // World matrix now controlled using the Transform Component    
public:
    Transform transform;

    GameObject();

    void attach_model(Model *m);

    void draw(SceneCamera &camera);

	void update();

    void pass(Shader *s);
};

