#pragma once
#include "../Mesh.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../../CompiledShaderProgram.h"

class Plane : public Mesh
{
private:
	static const int VERTEX_COUNT = 6;
	static const int VERTEX_ATTR_COUNT = 5;
	const float vertices[VERTEX_COUNT * VERTEX_ATTR_COUNT] = {
		// positions          // texture Coords 
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	void initIndex()
	{

	}
public:
	Plane() : Mesh()
	{
		std::vector<Vertex>& Vertices = GetVertex();
		Vertices.resize(VERTEX_COUNT);

		for (int i = 0; i < VERTEX_COUNT; i++) {
			int line = i * VERTEX_ATTR_COUNT;
			Vertices[i] = Vertex(
				vertices[line + 0], vertices[line + 1], vertices[line + 2],
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				vertices[line + 3], vertices[line + 4]);
		}
	}

	virtual void Draw(CompiledShaderProgram& shader) override
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}
};