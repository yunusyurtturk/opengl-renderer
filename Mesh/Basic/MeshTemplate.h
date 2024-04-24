#pragma once
#include "../Mesh.h"
#include <GL/glew.h>
#include <SDL_opengl.h>

class MeshTemplate : public Mesh
{
private:
	GLuint VBO = -1;
	GLuint IBO = -1;
	
public:
	vector<Texture> diffuseMaps;
	vector<Texture> specularMaps;
	vector<Texture> normalMaps;
	vector<Texture> heightMaps;

	MeshTemplate() : Mesh()
	{
		
	}
	virtual void Setup()
	{
		auto& textures = GetTextures();
		textures.insert(textures.end(),
			std::make_move_iterator(diffuseMaps.begin()),
			std::make_move_iterator(diffuseMaps.end()));
		textures.insert(textures.end(),
			std::make_move_iterator(specularMaps.begin()),
			std::make_move_iterator(specularMaps.end()));
		textures.insert(textures.end(),
			std::make_move_iterator(normalMaps.begin()),
			std::make_move_iterator(normalMaps.end()));
		textures.insert(textures.end(),
			std::make_move_iterator(heightMaps.begin()),
			std::make_move_iterator(heightMaps.end()));
		diffuseMaps.clear();
		specularMaps.clear();
		normalMaps.clear();
		heightMaps.clear();
		Mesh::Setup();
	}
	void SetDiffuseMaps(std::vector<Texture> &&source_diffuses)
	{
		diffuseMaps = source_diffuses;
	}

};