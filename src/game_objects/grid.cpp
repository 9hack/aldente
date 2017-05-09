#include "grid.h"
#include "events.h"
#include "game/phase.h"
#include "util/color.h"

#include <fstream>

#define FLOOR_TILE 3
#define WALL_TILE 5

// Default color is white, meaning that it only uses texture
Color default_color = Color::WHITE;
// When selected, will tint the tile green, but keeps texture
Color select_color = Color::GREEN;

Grid::Grid(const char *map_loc) :
        hover(nullptr), hover_col(0), hover_row(0) {
    load_map(map_loc);

    hover = grid[0][0];

    setup_listeners();
}

Grid::~Grid() {}

void Grid::setup_listeners() {
    events::build::build_grid_move_event.connect([&](Direction dir) {
        move_selection(dir);
    });

    events::build::build_grid_place_event.connect([&]() {
        proto::Construct c;
        c.set_type(selected);
        c.set_x(hover->getX());
        c.set_z(hover->getZ());
        events::build::request_build_event(c);
    });

    events::build::construct_changed_event.connect([&](ConstructType type) {
        selected = type;
    });

    events::build::try_build_event.connect([&](proto::Construct& c) {
        bool permitted = verify_build(static_cast<ConstructType>(c.type()), c.x(), c.z());
        c.set_status(permitted);
        // Build the construct locally on the server, without graphics.
        if (permitted) {
            Construct* built = build(static_cast<ConstructType>(c.type()), c.x(), c.z(), false);
            if (built)
                c.set_id(built->get_id());
        }

        events::build::respond_build_event(c);
    });

    events::build::update_build_event.connect([&](proto::Construct& c) {
        // Build on the client, with graphics.
        build(static_cast<ConstructType>(c.type()), c.x(), c.z(), true, c.id());
    });
}

void Grid::update() {
    if (grid[hover_row][hover_col] != hover) {
        hover->set_color(default_color);
        hover = grid[hover_row][hover_col];
        hover->set_color(select_color);
    }
}
bool Grid::verify_build(ConstructType type, int col, int row) {
    Tile* candidate = grid[row][col];
    if (type == REMOVE) {
        return candidate->get_construct() != nullptr;
    }

    return candidate->buildable;
}

Construct* Grid::build(ConstructType type, int col, int row, bool graphical, int id) {
    Tile* candidate = grid[row][col];
    Construct* to_add = nullptr;

    switch (type) {
    case CHEST: {
        to_add = new Crate(col, row);
        if (graphical) {
            to_add->setup_model();
            to_add->set_id(id);
        }
        candidate->set_construct(to_add);
        candidate->buildable = false;
        break;
    }
    case REMOVE: {
        // TODO: Move destructor to construct's destructor.
        if (candidate->get_construct() != nullptr) {
            events::remove_rigidbody_event(dynamic_cast<GameObject*>(candidate->get_construct()));
            candidate->set_construct(nullptr);
            candidate->buildable = true;
        }
        break;
    }
    default:
        break;
    }

    return to_add;
}

void Grid::move_selection(Direction d) {
    switch (d) {
    case UP:
        hover_row = glm::max(0, hover_row - 1);
        break;
    case RIGHT:
        hover_col = glm::min(width - 1, hover_col + 1);
        break;
    case DOWN:
        hover_row = glm::min(height - 1, hover_row + 1);
        break;
    case LEFT:
        hover_col = glm::max(0, hover_col - 1);
        break;
    default:
        break;
    }
}

void Grid::load_map(const char *map_loc) {
    // Loads map from file
    std::ifstream fin;
    fin.open(map_loc);

    // File does not exist, exit
    if (!fin.good())
        return;

    std::string str_buf;
    int int_buf;

    // Parse File
    while (!fin.eof()) {
        fin >> str_buf;
        if (str_buf == "height") {
            fin >> int_buf;
            height = int_buf;
        }
        else if (str_buf == "width") {
            fin >> int_buf;
            width = int_buf;
        }
        else if (str_buf == "tag") {
            // To implement later, if we want some automated way
            // in the file to check what number corresponds to what type of tile
            // Currently, tile_ids are hardcoded.
        }
        else if (str_buf == "data") {
            // Parses data and creates grid
            for (int r = 0; r < height; r++) {
                std::vector<Tile *> new_row;

                for (int c = 0; c < width; c++) {
                    fin >> int_buf;
                    new_row.push_back(make_tile(int_buf, c, r));
                }

                grid.push_back(new_row);
            }
        }
    }

    fin.close();
}

Tile *Grid::make_tile(int tile_id, int x, int z) {

    Tile *new_tile = nullptr;

    switch (tile_id) {
    case FLOOR_TILE:
        new_tile = new FloorTile(x, z);
        break;
    case WALL_TILE:
        new_tile = new WallTile(x, z);
        break;
    default:
        break;
    }

    return new_tile;
}

void Grid::graphical_setup() {
    for (std::vector<Tile*> row : grid) {
        for (Tile* tile : row) {
            tile->setup_model();
        }
    }
}
