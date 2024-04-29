#pragma once
#include "../Mesh.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../../CompiledShaderProgram.h"

class Quad : public Mesh
{
private:
	static const int VERTEX_COUNT = 6;
	static const int VERTEX_ATTR_COUNT = 4;
	const float vertices[VERTEX_COUNT * VERTEX_ATTR_COUNT] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	void initIndex()
	{

	}
public:
	Quad() : Mesh()
	{
		std::vector<Vertex>& Vertices = GetVertex();
		Vertices.resize(VERTEX_COUNT);

		for (int i = 0; i < VERTEX_COUNT; i++) {
			int line = i * VERTEX_ATTR_COUNT;
			Vertices[i] = Vertex(
				vertices[line + 0], vertices[line + 1], 0.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				vertices[line + 2], vertices[line + 3]);
		}
	}

	virtual void Draw(CompiledShaderProgram& shader) override
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
	}
};