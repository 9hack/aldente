/*
* Player Class:
* This class will contain all functionality concerning a player 
* gameobject such as health, gold, movement, etc.
*/

#include "game_object.h"
#include "game/direction.h"
#include "btBulletDynamicsCommon.h"

class Player : public GameObject {
private:
    // to_move is for saving the joystick input in each frame. This
    // logic will most likely change after the joystick changes.
    int to_moveX;
    int to_moveZ;

    // movement speed;
    float speed;

    btBoxShape *box = new btBoxShape(btVector3(0.5f, 0.001f, 0.5f));
    btRigidBody *rigidBody;
public:
    Player(Scene *scene);
    void update();
    void do_movement();
    void setup_listeners();
};