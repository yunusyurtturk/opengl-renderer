#pragma once
#include "../Mesh.h"
#include <GL/glew.h>
#include <SDL_opengl.h>

class Cube : public Mesh
{
private:

	void initCubeVertex()
	{
		std::vector<Vertex>& Vertices = GetVertex();
		Vertices.resize(8);

		Vertices[0] = Vertex(0.5f, 0.5f, 0.5f,    0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
		Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f,   0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		Vertices[3] = Vertex(0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
																	 				 
		Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		Vertices[5] = Vertex(0.5f, 0.5f, -0.5f,   0.5f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
		Vertices[6] = Vertex(0.5f, -0.5f, 0.5f,   0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f,  0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f);

		
	}
	void initCubeIndex()
	{
		std::vector<int>& Index = GetIndex();
		Index.resize(36);
		Index = {
			0, 1, 2,
			1, 3, 4,
			5, 6, 3,
			7, 3, 6,
			2, 4, 7,
			0, 7, 6,
			0, 5, 1,
			1, 5, 3,
			5, 0, 6,
			7, 4, 3,
			2, 1, 4,
			0, 2, 7
		};
	}
public:
	Cube() : Mesh()
	{
		initCubeVertex();
		initCubeIndex();
	}
};