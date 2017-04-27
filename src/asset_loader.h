#pragma once

#include "scene/scene.h"
#include "model/model.h"
#include "SOIL.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

/*
	Class for loading all assets on startup.
*/

class AssetLoader {
private:
    static void load(Model *model, std::string path);

    static void process_scene(Model *model, const aiScene *scene);
    static void process_node(Model *model, aiNode *node, const aiScene *scene);
    static void process_mesh(Model *model, aiMesh *mesh, const aiScene *scene);

    static std::map<std::string, Model *> models;
    static std::map<std::string, GLuint> textures;
public:
    static void setup();

    static Model *get_model(std::string name);
    static GLuint get_texture(std::string name);
};


