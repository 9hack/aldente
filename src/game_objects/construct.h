#pragma once

#include <glm/glm.hpp>
#include "game_object.h"
#include "btBulletDynamicsCommon.h"
#include "events.h"
#include "game/collectibles/collectible.h"

// Forward declare Collectible
namespace collectibles {
class Collectible;
}

// Note: Opengl has the x and z axis as its ground plane

class Construct : public GameObject {
protected:
    // The direction the construct is facing
    glm::vec3 forward;
public:
    Construct(int x, int z, int id = 0);

    virtual void setup_model() override {};

    virtual void s_interact_trigger(GameObject *other) override {};
    virtual void c_interact_trigger(GameObject *other) override {};
    virtual void s_on_collision(GameObject *other) override {};
    virtual void c_on_collision(GameObject *other) override {};

    // Does not actually physically rotate the construct, these
    // change the construct's forward vector
    void rotate_dir_left();
    void rotate_dir_right();
};

class Chest : public Construct {
public:
    Chest(int x, int z, int id = 0);

    void setup_model() override;
    
    void s_interact_trigger(GameObject *other) override;
    void c_interact_trigger(GameObject *other) override;

    void disappear(); // Causes chest to fade away
private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    std::unique_ptr<collectibles::Collectible> contents;

    std::function<void()> cancel_fade; // Callback function to cancel fading away.
};

class Goal : public Construct {
public:
    Goal(int x, int z, int id = 0);

    void setup_model() override;

    void s_on_collision(GameObject *other) override;
    void c_on_collision(GameObject *other) override;

private:
    btCylinderShape *goal_hit_box = new btCylinderShape(btVector3(0.005f, 0.5f, 0.005f));
};

class Spikes : public Construct {
public:
    Spikes(int x, int z, int id = 0);

    void setup_model() override;
    void s_on_collision(GameObject *other) override;
    void c_on_collision(GameObject *other) override;

private:
    btBoxShape *hit_box = new btBoxShape(btVector3(0.25f, 0.25f, 0.25f));
};
