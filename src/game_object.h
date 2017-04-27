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
public:
    Transform transform; // World matrix now controlled using the Transform Component    

    GameObject();

    void attach_model(Model *m);

    void draw(SceneCamera &camera);	
    void pass(Shader *s);

    virtual void update();
};

