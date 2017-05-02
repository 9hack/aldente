#pragma once

#include "scene/scene_info.h"
#include "model/model.h"
#include "transform.h"
#include "shaders/shader.h"
#include "animation/animation_player.h"

/*
    This represents an abstract object in the scene.
    Contains all the essential components required to
    render the model in the scene.
*/
class GameObject {
protected:
    Model *model;
    AnimationPlayer anim_player;

public:
    Transform transform; // World matrix now controlled using the Transform Component

    GameObject();

    void attach_model(Model *m);

    virtual void draw(Shader *shader, SceneInfo &scene_info);

    void update();

    void set_color(glm::vec3 color);

    Model* get_model() { return model; };
};

