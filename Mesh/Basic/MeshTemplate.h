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

	void SetDiffuseMaps(std::vector<Texture> &&source_diffuses)
	{
		diffuseMaps = source_diffuses;
	}

};