/*
* Grid Class:
*   This class holds a 2D array of Tile pointers that make up the map. Each Tile pointer
*   represents a coordinate on the map (x,z). We use z because we want the map to be parallel to
*   the "ground". The pointer points to a Tile object which occupies that space (note that tiles
*   can be more than a single coordinate big).
*/

#pragma once

#include <vector>
#include "tile.h"
#include "game/construct_types.h"
#include "game/direction.h"

class Grid {
private:
    void setup_listeners();

    std::vector<std::vector<Tile *>> grid; // Uses grid[row][column], or row[z][x]
    int width, height;
    int hoverX, hoverZ;
    Tile *hover;
    ConstructType selected = ConstructType::REMOVE;
public:
    Grid(const char *map_loc);

    ~Grid();

    std::vector<std::vector<Tile *>> getGrid() { return grid; };

    void load_map(const char *map_loc);
    Tile *make_tile(int tile_id, int x, int z);

    void update();

    // Returns true if this construct is allowed to be built.
    bool verify_build(ConstructType type, int row, int col);

    // Builds a construct at a location.
    void build(ConstructType type, int x, int z);

    void move_selection(Direction d);
};