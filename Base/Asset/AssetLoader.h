#pragma once

#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../Mesh/Mesh.h"
#include "../../Mesh/Basic/MeshTemplate.h"

class AssetLoader
{
private:
	std::string directory;
	vector<unique_ptr<Mesh>> *meshes;
	vector<Texture *> textures_loaded;
	std::filesystem::path full_path;
	std::filesystem::path parent_path;
public:
	AssetLoader(std::string const &path, vector<unique_ptr<Mesh>> &outMeshes, vector<Texture *> &outTextures): 
		full_path(path),
		parent_path(full_path.parent_path())
	{
		meshes = &outMeshes;
		textures_loaded = outTextures;
		LoadAsset(path);
	}
	void  LoadAsset(std::string const &path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
			cout << "ERROR: ASSIMP MODEL LOAD ERROR - " << importer.GetErrorString() << "\n";
			return;
		}
		directory = path.substr(0, path.find_last_of('\\'));

		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes->push_back(processMesh(mesh, scene));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	unique_ptr<MeshTemplate> processMesh(aiMesh* mesh, const aiScene* scene)
	{
		auto meshTemplate = make_unique<MeshTemplate>();
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			vertex.initPos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

			if (mesh->HasNormals())
			{
				vertex.initNormals(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			}

			if (mesh->mTextureCoords[0]) {
				vertex.initTexture(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

				if (mesh->mTangents && mesh->mTangents->Length() > 0) {
					vertex.initTangents(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
				}
				if (mesh->mBitangents && mesh->mBitangents->Length() > 0) {
					vertex.initBitangents(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
				}
			}
			else {
				vertex.initTexture(0.0f, 0.0f);
			}
			meshTemplate->AddVertex(std::move(vertex));
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				meshTemplate->AddIndex(face.mIndices[j]);
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		meshTemplate->diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,  "texture_diffuse");
		meshTemplate->specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		meshTemplate->normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS,   "texture_normal");
		meshTemplate->heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT,   "texture_height");
	
		meshTemplate->Setup();
		return meshTemplate;
	}

	std::vector<Texture *> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture *> textures;
		unsigned int count = mat->GetTextureCount(type);

		for (unsigned int i = 0; i < count; i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;

			auto full_path_str = (parent_path / str.C_Str()).string();
			for (auto j = 0; j < textures_loaded.size(); j++) {
				if (std::strcmp(textures_loaded[j]->getPath().c_str(), full_path_str.c_str()) == 0) {
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip) {
				Texture * texture = new Texture(full_path_str, typeName);
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}

		return textures;
	}
};