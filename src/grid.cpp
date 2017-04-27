#include "grid.h"

Grid::Grid(int w, int h) :
		hover(nullptr) {
    width = w;
    height = h;
    hoverX = 0;
    hoverZ = 0;

    for (int i = 0; i < width; i++) {
        std::vector<Tile *> newRow;
        for (int j = 0; j < height; j++) {
            if (i == 0 || i == width - 1 || j == 0 || j == height - 1) {
                WallTile* toAdd = new WallTile(i, j);
				newRow.push_back(toAdd);
            } else {
				FloorTile* toAdd = new FloorTile(i, j);
				newRow.push_back(toAdd);
            }
        }
        grid.push_back(newRow);
    }

    hover = grid[hoverX][hoverZ];
}

Grid::~Grid() {}

void Grid::update() {
    if (grid[hoverX][hoverZ] != hover) {
        hover->set_color(color::indian_red);
        hover = grid[hoverX][hoverZ];
        hover->set_color(color::windwaker_green);
    }
}

/*void Grid::build(bool is_chest) {

    if (is_chest) {
        //Crate* toAdd = new Crate(hover->getX(), hover->getZ());
        GameObject* toAdd = new GameObject();
        toAdd->model->meshes = crate->meshes;
        toAdd->model->model_mat[3] = glm::vec4(hover->getX(), 0, hover->getZ(), 1.0f);
        hover->add_construct(toAdd);
    }
    else {
        hover->add_construct(nullptr);
    }
}*/

/*void Grid::move_selection(GridDirection d) {
    switch (d) {
    case GridDirection::GUP:
        hoverZ = glm::max(0, hoverZ - 1);
        break;
    case GridDirection::GRIGHT:
        hoverX = glm::min(width - 1, hoverX + 1);
        break;
    case GridDirection::GDOWN:
        hoverZ = glm::min(height - 1, hoverZ + 1);
        break;
    case GridDirection::GLEFT:
        hoverX = glm::max(0, hoverX - 1);
        break;
    default:
        break;
    }
}*/

