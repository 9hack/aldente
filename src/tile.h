/*
* Tile Class:
*   This file holds the superclass of Tile and all of its subclasses. The superclass Tile holds basic
*   varaibles and functions that all Tiles will require in order to function. These include, and are not
*   limited to: width, height, x coord, z coord, Mesh, getter functions. Each actual Tile will then
*   be a child of this superclass and implement the neccessary functionality for that specific tile
*   (for example: rendering, collision detection, trigger functionality).
*/

#pragma once

#include "model/mesh.h"
#include "model/material.h"
#include "model/geometry_generator.h"
#include "btBulletDynamicsCommon.h"
#include "game_object.h"

class Tile : public GameObject {
protected:
    int width, height, x, z;
    bool traversable;
    btRigidBody *rigidBody;
    btBoxShape *box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    btBoxShape *ground = new btBoxShape(btVector3(0.5f, 0.05f, 0.5f));

private:

public:
    Tile();

    ~Tile();

    //Mesh *getMesh() { return mesh; };

    int getX() { return x; };

    int getZ() { return z; };

    virtual void update(Tile *hover) = 0;

    btRigidBody *getRigid() { return rigidBody; };
};

class FloorTile : public Tile {

public:
    FloorTile(int x, int z);

    ~FloorTile();

    void update(Tile *hover);
};

class WallTile : public Tile {

public:
    WallTile(int x, int z);

    ~WallTile();

    void update(Tile *hover);
};
