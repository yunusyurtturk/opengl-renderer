#pragma once
#include "../Model.h"
#include <GL/glew.h>
#include <SDL_opengl.h>

class Cube : public Model
{
private:
	GLuint VBO = -1;
	GLuint IBO = -1;

	void initVertex()
	{
		std::vector<Vertex>& Vertices = GetVertex();
		Vertices.resize(8);

		Vertices[0] = Vertex(0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f);
		Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
		Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertices[3] = Vertex(0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f);

		Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertices[5] = Vertex(0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f);
		Vertices[6] = Vertex(0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);

		int sizeofVertex = sizeof(Vertex);
		int sizeofArray = sizeof(Vertices);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);



		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		// texture
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
	}
	void initIndex()
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
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Index.size() * sizeof(int), &Index[0], GL_STATIC_DRAW);
	}
public:
	Cube() : Model()
	{
		initVertex();
		initIndex();
	}

	virtual void Draw() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
};