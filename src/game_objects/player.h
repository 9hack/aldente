/*
* Player Class:
* This class will contain all functionality concerning a player 
* gameobject such as health, gold, movement, etc.
*/

#include "game_object.h"
#include "game/direction.h"
#include "btBulletDynamicsCommon.h"
#include "construct.h"

class Player : public GameObject {
private:
    glm::vec3 direction;

    // to_move is for saving the joystick input in each frame. This
    // logic will most likely change after the joystick changes.
    int to_moveX;
    int to_moveZ;

    // movement speed;
    float speed;

    btCapsuleShape *capsule = new btCapsuleShape(0.5f, 1.0f);
public:
    Player();
    void update() override;
    void do_movement();
    void setup_listeners();
    void interact();
};