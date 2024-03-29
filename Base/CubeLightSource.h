#pragma once
#include <array>
#include "Primitive.h"
#include "../Vertex.h"

class CubeLightSource : public Primitive
{
private:
	std::array<unsigned int, 36> m_indices;
	Vertex Vertices[8];
	GLuint VBO = -1;
	GLuint IBO = -1;
	GLuint objectColorLocation = -1;
	GLuint lightColorLocation = -1;
	glm::vec3 objectColor = glm::vec3(0.0f, 0.5f, 0.5f);
	glm::vec3 lightColor = glm::vec3(0.0f, 1.0f, 1.0f);
public:
	CubeLightSource() : Primitive()
	{
		SetVertices();
		SetIndexBuffer();
	}
	virtual void AddShader(CompiledShaderProgram shader) {
		m_Shaders[m_ShaderIndex] = shader;
		m_ShaderIndex++;

		objectColorLocation = glGetUniformLocation(shader.ShaderProgram, "objectColor");
		if (objectColorLocation == -1) {
			printf("Error getting uniform location 'objectColor'\n");
			exit(1);
		}

		lightColorLocation = glGetUniformLocation(shader.ShaderProgram, "lightColor");
		if (lightColorLocation == -1) {
			printf("Error getting uniform location 'lightColor'\n");
			exit(1);
		}
	}
	void SetIndexBuffer()
	{
		m_indices = {
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), &m_indices[0], GL_STATIC_DRAW);
	}

	void SetVertices()
	{
		Vertices[0] = Vertex(0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f);
		Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
		Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertices[3] = Vertex(0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f);

		Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertices[5] = Vertex(0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f);
		Vertices[6] = Vertex(0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	}

	virtual void Update(glm::mat4x4& vp) override
	{
		Primitive::Update(vp);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		if (objectColorLocation != -1) {
			glUniform3fv(objectColorLocation, 1,  glm::value_ptr(objectColor));
		}
		if (lightColorLocation != -1) {
			glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
		}

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
};