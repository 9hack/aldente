#include "construct.h"
#include "asset_loader.h"

Construct::Construct(int x, int z) : GameObject() {
    transform.set_position(x, 0.0f, z);
}

Chest::Chest(int x, int z) : Construct(x, z) {
    transform.set_scale(0.6f, 0.6f, 0.6f);

    events::RigidBodyData rigid = {
        glm::vec3(x,0.5f,z), //position
        0, //mass
        hit_box, //btshape
        glm::vec3(0,0,0), //inertia
        this, //the gameobject
        true, // is a ghost object
    };
    events::add_rigidbody_event(rigid);
}

// Activated when a player presses A on it
void Chest::interact_trigger() {
    // Will change after server/client split
    attach_model(AssetLoader::get_model("veggie"));
}

void Chest::setup_model() {
    attach_model(AssetLoader::get_model("chest_good"));
    transform.set_scale({ 0.005f, 0.005f, 0.005f });
}

Goal::Goal(int x, int z) : Construct(x, z) {
    transform.set_scale(0.6f, 0.6f, 0.6f);

    events::RigidBodyData rigid = {
        glm::vec3(x,0.5f,z), //position
        0, //mass
        hit_box, //btshape
        glm::vec3(0,0,0), //inertia
        this, //the gameobject
        true, // is a ghost object
    };
    events::add_rigidbody_event(rigid);
}

void Goal::interact_trigger() {
    anim_player.play();
}

void Goal::setup_model() {
    attach_model(AssetLoader::get_model("warp"));
    model->set_shader(&ShaderManager::anim_unlit);
    anim_player.set_speed(1.0f);
    anim_player.set_anim(&skel, "spin");
    anim_player.set_loop(false);
}
