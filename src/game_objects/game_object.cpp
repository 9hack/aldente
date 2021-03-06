#include "game_object.h"
#include "util/util_bt.h"
#include <iostream>
#include "events.h"

std::unordered_map<int, GameObject*> GameObject::game_objects;
int GameObject::id_counter = STARTING_ID;

GameObject::GameObject(int id) : id(id) {

    if (id == REQUIRE_ID) {
        // This object is new and being made on the server
        this->id = id_counter++;
    }

    model = new Model();
    rigidbody = nullptr;

    if (id != -1)
        game_objects[this->id] = this;
    
    enabled = true;
    use_rb_transform = false;
}

GameObject::~GameObject(){

    //TODO : Cleanup for rigid bodies and removing from game

    for (GameObject *obj : children)
        delete(obj);
}

// Adopt a game object
void GameObject::add_child(GameObject *obj) {
    children.push_back(obj);
}

// Disown a single child (Note, this does not delete the object, in case of reparenting)
void GameObject::remove_child(GameObject *obj) {
    // I'm pretty sure this line of code works. Not tested.
    children.erase(std::remove(children.begin(), children.end(), obj), children.end());
}

// Get rid of all the children (Note, this deletes the object.)
void GameObject::remove_all() {
    for (GameObject *obj : children)
        delete(obj);
    children.clear();
}

// Renders model and children's models in scene
void GameObject::c_draw(Shader *shader, SceneInfo &scene_info) {
    if (!enabled) return;

    if (model) {
        connect_model_filter();
        model->draw(shader, scene_info, transform.get_world_mat());
    }

    for (GameObject *obj : children)
        obj->c_draw(shader, scene_info);
}

// Draw multiple instances of this GameObject, which should have no children.
void GameObject::c_draw_instanced(Shader *shader, SceneInfo &scene_info,
                      std::vector<glm::mat4> instance_matrix) {
    if (model) {
        connect_model_filter();
        model->draw_instanced(shader, scene_info, instance_matrix);
    }
}

// Runs Game Object's update loop
void GameObject::s_update() {
    if (!enabled) return;

    s_update_this();

    for (GameObject *obj : children)
        obj->s_update();
}

void GameObject::c_update() {
    if (!enabled) return;

    c_update_this();

    anim_player.update();

    for (GameObject *obj : children)
        obj->c_update();
}

void GameObject::c_update_state(glm::mat4 mat, bool enab) {
    if (enab)
        enable();
    else
        disable();

    if (enabled) {
        transform.set_world_mat(mat);
        set_rb_transform();
        transform.set_scale(initial_transform.get_scale());
    }
}

void GameObject::attach_model(Model *m) {
    model = m;
    model_filter.skeleton = m->initial_skeleton;
    anim_player.set_skeleton(&model_filter.skeleton);
}

// Attaches bones from skeleton to model, as well as an overriding material for the model.
// Allows for different game objects using the same model
// to be animated independently. This needs to happen before each
// one's draw call (i.e. cannot just be done in update)
void GameObject::connect_model_filter(){
    model->set_model_filter(model_filter);
}

void GameObject::disable_filter() {
    model_filter.override_alpha = false;
    model_filter.override_diffuse = false;
    model_filter.override_shadows = false;
}

void GameObject::set_filter_color(Color color) {
    model_filter.material.diffuse = color;
    model_filter.override_diffuse = true;
}

void GameObject::set_filter_alpha(float alpha) {
    model_filter.material.alpha = alpha;
    model_filter.override_alpha = true;
}

void GameObject::set_filter_shadows(bool enable) {
    model_filter.material.shadows = enable;
    model_filter.override_shadows = true;
}

// Sets the material color for the entire game object's model
void GameObject::set_color(Color color) {
    model->set_color(color);
}

void GameObject::set_alpha(float alpha) {
    model->set_alpha(alpha);
}

void GameObject::set_shadows(bool enable) {
    model->set_shadows(enable);
}

// Sets position of both object's transform and rigid body
// Use this intead of transform.set_position to make sure that rigid body
// is located in the same place as the object
void GameObject::set_position(glm::vec3 pos) {
    // Set for Transform
    transform.set_position(pos);
    set_rb_transform();
}

// Sets scale for both transform and initial transform, since
// for the client, it needs to be properly saved somewhere in order to maintain
// its scale in c_update_state
void GameObject::set_scale(glm::vec3 scale) {
    transform.set_scale(scale);
    initial_transform.set_scale(scale);
}

void GameObject::set_rb_transform() {
    // Set for Rigid Body
    btTransform initialTransform;

    if (use_rb_transform)
        initialTransform.setFromOpenGLMatrix(glm::value_ptr(transform.get_world_mat()));

    if (rigidbody) {
        if (!use_rb_transform) {
            initialTransform.setOrigin(util_bt::convert_vec3(transform.get_position()));
            initialTransform.setRotation(btQuaternion::getIdentity());
        }
        rigidbody->setWorldTransform(initialTransform);
        rigidbody->getMotionState()->setWorldTransform(initialTransform);
    }
}

void GameObject::sync_position() {
    // Update transform with bullet's
    btTransform t;
    rigidbody->getMotionState()->getWorldTransform(t);
    btVector3 to_set = t.getOrigin();

    // If asserts fails, then some bullet vector/transform somewhere
    // isn't being initialized properly. (probably)
    assert(!std::isnan(to_set.getX()));
    assert(!std::isnan(to_set.getZ()));

    transform.set_position(util_bt::convert_vec3(to_set));
}

void GameObject::reset_to_initial_transform() {
    transform = initial_transform;
    set_rb_transform();
}

void GameObject::set_ghost(bool b) {
    if (!rigidbody)
        return;

    if (b)
        rigidbody->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
    else
        rigidbody->setCollisionFlags(0);
}

void GameObject::disable() {
    if (enabled) {
        enabled = false;
        if (rigidbody) {
            events::disable_rigidbody_event(this);
        }
    }
}

void GameObject::enable() {
    if (!enabled) {
        enabled = true;
        if (rigidbody) {
            events::enable_rigidbody_event(this);
        }
    }
}

void GameObject::disable_all() {
    if (enabled) {
        enabled = false;
        if (rigidbody) {
            events::disable_rigidbody_event(this);
        }
        for (GameObject *obj : children) {
            obj->disable_all();
        }
    }
}

void GameObject::enable_all() {
    if (!enabled) {
        enabled = true;
        if (rigidbody) {
            events::enable_rigidbody_event(this);
        }
        for (GameObject *obj : children) {
            obj->enable_all();
        }
    }
}

void GameObject::set_initial_direction(glm::vec3 dir) {
    transform.look_at(dir);
    initial_transform.look_at(dir);
}