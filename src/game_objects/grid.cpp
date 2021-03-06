#include "grid.h"
#include "audio/audio_manager.h"
#include "events.h"
#include "game/phase/phase.h"
#include "util/color.h"
#include "util/path_finding.h"
#include "util/util.h"

#include "game_objects/traps/spikes.h"
#include "game_objects/traps/slime.h"
#include "game_objects/traps/arrow_trap.h"
#include "game_objects/traps/mimic.h"
#include "game_objects/traps/slow_trap.h"
#include "game_objects/traps/shooter.h"
#include "game_objects/traps/bomb.h"
#include "game_objects/traps/confuse_trap.h"

#include <fstream>

#define FLOOR_TILE 0
#define EMPTY_TILE 3
#define TREE_TILE 5
#define BIG_ROCK 6
#define SMALL_ROCK 7
#define GRASS 8

#define PADDING 20 // Padding along side of dungeon for grass and trees

#define STARTING_HOVER 20

Grid::Grid(std::string map_loc) :
        hover(nullptr), hover_col(STARTING_HOVER), hover_row(STARTING_HOVER),
        width(0), height(0) {

    tag = "GRID";
    model = nullptr;
    rigidbody = nullptr;
    build_permissible = true;

    load_map(map_loc);
    fill_grass();
    fill_trees();

    // Default starting location
    hover = grid[hover_col][hover_row];

    setup_listeners();
}

// Override draw - using instanced rendering for performance purposes.
void Grid::c_draw(Shader *shader, SceneInfo &scene_info) {
    // Draw all types of tiles using instanced rendering.
    for (auto it = tile_types.begin(); it != tile_types.end(); ++it) {
        std::vector<Tile *> & vec = it->second;
        // Do instanced draw call on first tile
        vec[0]->c_draw_instanced(shader, scene_info); // do not pass in instance matrix because it doesn't change
    }

    // Draw all constructs by calling base class function.
    GameObject::c_draw(shader, scene_info);
}

void Grid::setup_listeners() {

    // When moving selection during build phase
    events::build::build_grid_move_event.connect([&](Direction dir) {
        move_selection(dir);
        update_selection();
        events::build::hover_position_updated_event(hover_col, hover_row);
    });

    events::build::build_grid_place_event.connect([&]() {
        proto::Construct c;
        c.set_type(selected);
        c.set_x(hover->getX());
        c.set_z(hover->getZ());
        c.set_fwd_x(preview.curr_preview->transform.get_forward().x);
        c.set_fwd_y(preview.curr_preview->transform.get_forward().y);
        c.set_fwd_z(preview.curr_preview->transform.get_forward().z);

        // Client-side early verification. Check if this location is buildable for sound effect.
        if (!verify_build(static_cast<ConstructType>(c.type()), c.x(), c.z()))
            events::sound_effects_event(events::AudioData(AudioManager::INVALID_BUZZ_SOUND, false));

        events::build::request_build_event(c);
    });

    events::build::c_construct_preview_event.connect([&](ConstructType type, bool valid) {
        selected = type;
        build_permissible = valid;

        // Change preview to this construct type.
        preview.set_construct_type(type, valid);
        children.push_back(&preview);
        // Update preview position based on hover position
        update_selection();
    });

    events::build::c_rotate_preview_event.connect([&](bool cw) {
        if (cw)
            preview.curr_preview->transform.rotate(glm::vec3(0, -90, 0));
        else
            preview.curr_preview->transform.rotate(glm::vec3(0, 90, 0));
    });

    events::c_player_stats_updated.connect([&](const proto::PlayerStats &update) {
        build_permissible = update.coins() >= Constructs::CONSTRUCTS.at(selected).cost;
        preview.set_valid(hover->buildable && build_permissible);
    });

    events::build::select_grid_return_event.connect([&]() {
        // Remove preview from children
        remove_child(&preview);
    });

    events::build::s_try_build_event.connect([&](proto::Construct& c, std::function<void()> success) {
        bool permitted = verify_build(static_cast<ConstructType>(c.type()), c.x(), c.z());
        c.set_status(permitted);
        // Build the construct locally on the server, without graphics.
        // A build is permitted if the desired tile is buildable, e.g. not a wall and has no existing construct.
        if (permitted) {
            Construct* built = build(static_cast<ConstructType>(c.type()), c.x(), c.z(), c.fwd_x(), c.fwd_y(), c.fwd_z());
            if (built) {
                c.set_id(built->get_id());
            }
            success();
        }

        events::build::respond_build_event(c);
    });

    events::build::update_build_event.connect([&](proto::Construct& c) {
        // Build on the client, with graphics.
        build(static_cast<ConstructType>(c.type()), c.x(), c.z(), c.fwd_x(), c.fwd_y(), c.fwd_z(), c.id());

        events::sound_effects_event(events::AudioData(AudioManager::BUILD_CONFIRM_SOUND, false));
    });

    events::build::end_build_event.connect([&]() {
        // Removes any existing preview when leaving build phase.
        remove_child(&preview);
        // Reset hover
        hover_col = hover_row = STARTING_HOVER;
    });
}

bool Grid::verify_build(ConstructType type, int col, int row) {
    Tile* candidate = grid[row][col];
    if (type == REMOVE) {
        return candidate->get_construct() != nullptr;
    }

    return candidate->buildable;
}

Construct* Grid::build(ConstructType type, int col, int row, float fx, float fy, float fz, int id) {
    Tile* candidate = grid[row][col];
    Construct* to_add = nullptr;

    if (type == REMOVE) {
        // TODO: Move destructor to construct's destructor.
        if (candidate->get_construct() != nullptr) {
            if (id == ON_SERVER) {
                events::remove_rigidbody_event(dynamic_cast<GameObject*>(candidate->get_construct()));
                candidate->buildable = true;
                candidate->set_construct(nullptr);
            }
            else {
                remove_child(candidate->get_construct());
                candidate->set_construct(nullptr);
            }
        }
    }
    else if (type != NONE) {
        switch (type) {
        case CHEST:
            to_add = new Chest(col, row, id);
            break;
        case MIMIC:
            to_add = new Mimic(col, row, id);
            break;
        case BOMB:
            to_add = new Bomb(col, row, id);
            break;
        case SPIKES:
            to_add = new Spikes(col, row, id);
            break;
        case SLIME_B:
            to_add = new SlimeBlue(col, row, id);
            break;
        case SLIME_Y:
            to_add = new SlimeYellow(col, row, id);
            break;
        case SLIME_R:
            to_add = new SlimeRed(col, row, id);
            break;
        case SLIME_G:
            to_add = new SlimeGreen(col, row, id);
            break;
        case ARROW_TRAP:
            to_add = new ArrowTrap(col, row, id);
            break;
        case SHOOTER:
            to_add = new Shooter(col, row, id);
            break;
        case ICE:
            to_add = new SlowTrap(col, row, id);
            break;
        case CONFUSE:
            to_add = new ConfuseTrap(col, row, id);
            break;
        default:
            return nullptr;
            break;
        }
    
        if (id != ON_SERVER)
            to_add->setup_model();

        to_add->set_initial_direction(glm::vec3(fx, fy, fz));
        children.push_back(to_add);
        candidate->set_construct(to_add);
        candidate->buildable = false;
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

// Updates Selected Tile
void Grid::update_selection() {
    hover = grid[hover_row][hover_col];

    // Move preview to selected tile
    preview.curr_preview->transform.set_position(hover_col, 0.0f, hover_row);
    preview.set_valid(hover->buildable && build_permissible);
}

void Grid::load_map(std::string map_loc) {
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
                    Tile *new_tile = make_tile(int_buf, c, r);
                    new_row.push_back(new_tile);
                }
                grid.push_back(new_row);
            }
        }
    }

    fin.close();

    // Setup Grid for Path Planning
    PathFinding::set_grid(grid);
}

Tile *Grid::make_tile(int tile_id, int x, int z) {

    Tile *new_tile = nullptr;

    int rand_id;

    switch (tile_id) {
    case FLOOR_TILE:
        new_tile = new FloorTile(x, z);
        break;
    case TREE_TILE:
        new_tile = new TreeTile(x, z);
        break;
    case EMPTY_TILE:
        // Random environment tile
        rand_id = random_env_tile();
        if (rand_id == BIG_ROCK)
            new_tile = new BigRockTile(x, z);
        else if (rand_id == SMALL_ROCK)
            new_tile = new SmallRockTile(x, z);
        else if (rand_id == GRASS)
            new_tile = new GrassTile(x, z);
        else {
            new_tile = new EmptyTile(x, z);
            return new_tile; // Don't need to save tile
        }
        tile_id = rand_id;
        break;
    default:
        break;
    }

    // Push pointer back to tile_types map.
    tile_types[tile_id].push_back(new_tile);

    return new_tile;
}

void Grid::fill_grass() {

    const int padding = PADDING;

    // Adds grass tiles to entire grid with padding on the sides
    for (int r = -padding; r < height + padding; r++) {
        for (int c = -padding; c < width + padding; c++) {
            Tile *new_tile = make_tile(FLOOR_TILE, c, r);
        }
    }
}

void Grid::fill_trees() {
    const int padding = PADDING;

    // Adds tree tiles to outer areas of grid
    for (int r = -padding; r < height + padding; r++) {
        for (int c = -padding; c < width + padding; c++) {
            if (r >= 0 && r < height && c >= 0 && c < width)
                continue;
            // 20% tree population
            if (static_cast<int>(Util::random(0.f, 5.f)))
                continue;

            Tile *new_tile = new EmptyTreeTile(c, r);
            tile_types[TREE_TILE].push_back(new_tile);
        }
    }
}

void Grid::setup_model() {
    // Only setup model once for each tile type.
    // Use first element of each tile type vector to set up model.
    for (auto it = tile_types.begin(); it != tile_types.end(); ++it) {
        std::vector<Tile *> & vec = it->second;

        std::vector<glm::mat4> instance_matrix; // vector of all tile transforms
        for (Tile * t : vec) {
            // Insert this tile's transform world matrix into the vector
            instance_matrix.push_back(t->transform.get_world_mat());
        }

        vec[0]->setup_instanced_model(vec.size(), instance_matrix);
    }
}

int Grid::random_env_tile(){
    // Partitions between 0 - 100 for random chances
    int rand_num = (int) Util::random(0, 100);

    if (rand_num > 95) // 5% chance grass
        return GRASS;
    else if (rand_num > 93) // 2% chance small rocks
        return SMALL_ROCK;
    else if (rand_num > 91) // 2% chance big rock
        return BIG_ROCK;
    else    // 50% chance nothing
        return EMPTY_TILE;
}
