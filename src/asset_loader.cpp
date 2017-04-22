#include "asset_loader.h"
#include "aldente.h"
#include "util/colors.h"
#include "shaders/shader_manager.h"

#include <boost/range.hpp>
#include <boost/filesystem.hpp>

AssetLoader *AssetLoader::asset_loader = new AssetLoader();

AssetLoader::AssetLoader() {}
AssetLoader::~AssetLoader() {}

void AssetLoader::setup()
{
	boost::filesystem::path path = boost::filesystem::path("assets/fbx");
	for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(path), {})) {
        std::size_t found = entry.path().filename().string().find_first_of("@");

		std::string filepath = std::string("assets/fbx/");
		filepath += entry.path().filename().string();

        // If @ is not in filename (meaning that its a model and not an animation)
        if (found == string::npos) {
            model = new Model();
            load(filepath,true);
            assets[entry.path().filename().string().c_str()] = model;
        }
        // Else it is an animation
        else {
            load(filepath,false);
        }
	}

	// Test
}

void AssetLoader::load(std::string path, bool isModel)
{
	unsigned int processFlags =
		aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
		aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
										  //aiProcess_ValidateDataStructure  | // perform a full validation of the loader's output
		aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
		//aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
		aiProcess_SortByPType | // ?
		aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
		aiProcess_RemoveRedundantMaterials | // remove redundant materials
		aiProcess_FindDegenerates | // remove degenerated polygons from the import
		aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
		aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
		aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
		aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
		aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
		aiProcess_OptimizeMeshes | // join small meshes, if possible;
		aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
		aiProcess_PreTransformVertices | //-- fixes the transformation issue.
		//aiProcess_FlipUVs | // flips UV coords
		0;

	const aiScene* scene = import.ReadFile(path, processFlags);

	if (!scene)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	//this->directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene, isModel);
}

void AssetLoader::processNode(aiNode* node, const aiScene* scene, bool isModel)
{
	// Gets all meshes and adds it to the model

	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        if (isModel) {
            model->add_mesh(processMesh(mesh, scene));
        }
        else {
            model->meshes[0]->inverseBoneMat = scene->mRootNode->mTransformation;
            model->meshes[0]->inverseBoneMat.Inverse();
        }
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, isModel);
	}
}

Mesh* AssetLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// Geometry handles all the vertex buffers and stuff
	Geometry *geo = new Geometry();

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 vec;
		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		geo->vertices.push_back(vec);

		vec.x = mesh->mNormals[i].x;
		vec.y = mesh->mNormals[i].y;
		vec.z = mesh->mNormals[i].z;
		geo->normals.push_back(vec);

		if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			geo->tex_coords.push_back(vec);
		}
		else
			geo->tex_coords.push_back(glm::vec2(0.0f, 0.0f));
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			geo->indices.push_back(face.mIndices[j]);
		}
	}

	//Textures and Materials not yet loaded

	Mesh *final_mesh;

	// If material exists for this mesh
	if (mesh->mMaterialIndex >= 0) {
		Material *loadMat = new Material();
		aiMaterial* assimpMat = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D diffuse(0, 0, 0);
		assimpMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		aiColor3D ambient(0, 0, 0);
		assimpMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
		aiColor3D specular(0, 0, 0);
		assimpMat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
		float shiny = 0.0f;
		assimpMat->Get(AI_MATKEY_SHININESS, shiny);
		loadMat->ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
		loadMat->diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
		loadMat->specular = glm::vec3(specular.r, specular.g, specular.b);
		loadMat->shininess = shiny;
		final_mesh = new Mesh(geo, loadMat, ShaderManager::get_default());
		final_mesh->to_world = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f));

		//If textures exist for this mesh
		for (GLuint i = 0; i < assimpMat->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			aiString str;
			assimpMat->GetTexture(aiTextureType_DIFFUSE, i, &str);

            std::string temp(str.C_Str());
            std::size_t found = temp.find_last_of("/\\");
            std::string fileName = temp.substr(found + 1);
            std::string toPass("assets/textures/");
            toPass += fileName;

			//Texture not loaded yet
			if (textures.count(toPass) == 0) {
				geo->attachNewTexture(toPass.c_str());
				textures[toPass] = geo->getTextureGL();
			}
			else {
				geo->attachExistingTexture(textures[toPass]);
			}
		}
	}
	else {
		Material *blank = new Material(color::white);
		final_mesh = new Mesh(geo, blank, ShaderManager::get_default());
	}

	geo->populate_buffers();

	return final_mesh;
}


//Use this function to access a model, pass in a path in the form of
//"assets/fbx/the_model_you_want_here.fbx"
Model *AssetLoader::getModel(std::string name) {
	if (assets[name] == NULL) {
		std::string error("ERROR: Asset ");
		error += name;
		error += " was not loaded. Check for fbx file and double check filename.\n";
		fprintf(stderr, error.c_str());
		Model* dflt = new Model();
		return dflt;
	}
	return assets[name];
}
