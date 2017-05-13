#include "construct.h"
#include "asset_loader.h"

Construct::Construct(int x, int z) : GameObject() {
    tag = Tag::CONSTRUCT;
    transform.set_position(x, 0.0f, z);
}

Construct::Construct(int x, int z, int id) : GameObject(id) {
    tag = Tag::CONSTRUCT;
    transform.set_position(x, 0.0f, z);
}

Crate::Crate(int x, int z) : Construct(x, z) {
    transform.set_scale(0.6f, 0.6f, 0.6f);
    events::RigidBodyData rigid = {
        glm::vec3(x,0.5f,z), //position
        0, //mass
        hit_box, //btshape
        glm::vec3(0,0,0), //inertia
        this, //the gameobject
        false, // is a ghost object
    };
    events::add_rigidbody_event(rigid);
}

Crate::Crate(int x, int z, int id) : Construct(x, z, id) {
    transform.set_scale(0.6f, 0.6f, 0.6f);
}


void Crate::update_this() {
    anim_player.update();
}

// Activated when a player presses A on it
void Crate::interact_trigger() {
    anim_player.set_anim(&skel, "open");
    anim_player.play();
}

void Crate::setup_model() {
    attach_model(AssetLoader::get_model("chest_good"));
    transform.set_scale({ 0.005f, 0.005f, 0.005f });
}
