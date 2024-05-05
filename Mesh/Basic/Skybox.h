#pragma once
#include "../Mesh.h"
#include <GL/glew.h>
#include <SDL_opengl.h>
#include "../../CompiledShaderProgram.h"

class Skybox : public Mesh
{
private:
	static const int VERTEX_COUNT = 36;
    static const int VERTEX_ATTR_COUNT = 3;
	const float vertices[VERTEX_COUNT * VERTEX_ATTR_COUNT] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

	void initIndex()
	{

	}
public:
	Skybox() : Mesh()
	{
		std::vector<Vertex>& Vertices = GetVertex();
		Vertices.resize(VERTEX_COUNT);

		for (int i = 0; i < VERTEX_COUNT; i++) {
			int line = i * VERTEX_ATTR_COUNT;
			Vertices[i] = Vertex(
				vertices[line + 0], vertices[line + 1], vertices[line + 2],
				0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f);
		}
	}

	virtual void Draw(CompiledShaderProgram& shader) override
	{
		glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
		glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
		glBindVertexArray(0);

		
	}
};