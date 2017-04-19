#pragma once


#include "scene/scene.h"
#include "scene/scene_model.h"
#include "util/colors.h"
#include "shaders/shader_manager.h"


#include <boost/range.hpp>
#include <boost/filesystem.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

class AssetLoader {
private:
	map<string, SceneModel*> assets;
	map<string, GLuint> textures;
	SceneModel* model;
    Assimp::Importer import;
public:
	AssetLoader();
	~AssetLoader();

	void load(std::string path, bool isModel);
	void processNode(aiNode* node, const aiScene* scene, bool isModel);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	SceneModel* getModel(std::string name);
};


