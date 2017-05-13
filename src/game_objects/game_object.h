#pragma once

#include "scene/scene_info.h"
#include "transform.h"
#include "shaders/shader.h"
#include "animation/animation_player.h"
#include "btBulletDynamicsCommon.h"
#include "util/color.h"

#include <unordered_map>
#include <string>

/*
    This represents an abstract object in the scene.
    Contains all the essential components required to
    render the model in the scene.
*/
enum Tag {
    PLAYER, CONSTRUCT, TILE, GRID
};

class GameObject {
private:
    static int id_counter;
protected:
    Model *model;
    Skeleton skel;
    AnimationPlayer anim_player;
    btRigidBody *rigidbody;
    int id;
public:
    static std::unordered_map<int, GameObject*> game_objects;
    std::vector<GameObject *> children;

    Transform transform; // World matrix now controlled using the Transform Component
    glm::vec3 direction;
    Tag tag; // Identify this GameObject by a tag.
    bool notify_on_collision = false; // Physics engine will only call on_collision if this flag is set.

    GameObject();
    GameObject(int id);
    ~GameObject();

    // Parenting Stuff
    void add_child(GameObject *obj);
    void remove_child(GameObject *obj);
    void remove_all();

    void draw(Shader *shader, SceneInfo &scene_info);
    void update(); // Updates this object and all children

    virtual void update_this() {}; // Update function for this particular object. Use this instead of update()
    void update_state(float x, float z, float wx, float wz);
    virtual void on_collision(GameObject *other) {}
    virtual void on_collision_graphical() {}
    virtual void setup_model() {};

    void connect_skel_to_model();

    void set_color(Color color);
    void set_alpha(float alpha);
    void set_shadows(bool enable);

    Model* get_model() { return model; };
    Skeleton get_skeleton() { return skel; };
    void attach_model(Model *m) { model = m; skel = m->initial_skeleton; };

    btRigidBody *get_rigid() { return rigidbody; };
    void set_rigid(btRigidBody *to_add) { rigidbody = to_add; };
    int get_id() { return id; };

    void set_position(glm::vec3 pos);
};
