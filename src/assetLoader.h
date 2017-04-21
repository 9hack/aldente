#pragma once

#include "scene/scene.h"
#include "model\model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

/*
	Class for loading all assets on startup.
*/
class AssetLoader {
private:
	static map<string, Model*> assets;
	static map<string, GLuint> textures;
	static Model* model;
    static Assimp::Importer import;
public:	
	static void setup();
	static void load(std::string path, bool isModel);
	static void processNode(aiNode* node, const aiScene* scene, bool isModel);
	static Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
	static Model* getModel(std::string name);
};


