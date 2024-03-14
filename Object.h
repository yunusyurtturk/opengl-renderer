#ifndef MODEL_OBJECT_H
#define MODEL_OBJECT_H

#include "Texture.h"
#include "WorldTransform.h"
#include "Vertex.h"

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4

#include <array>
#include <memory>


class ModelObject
{
private:
	std::unique_ptr<Texture> m_texture;
	WorldTrans m_transform;
	Vertex Vertices[8];

	GLuint VAO = -1;
	GLuint VBO = -1;
	GLuint IBO = -1;
	GLuint texture = -1;
	GLuint gWLocation = -1;
	GLuint gSamplerLocation = -1;
	GLuint gViewLocation = -1;

	unsigned int m_ShaderIndex = 0;
	std::array<CompiledShaderProgram, 8> m_Shaders;
	std::array<unsigned int, 36> m_indices;
public:
	ModelObject()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		m_transform.SetPosition(0.0f, 0.0f, 2.0f);
		m_transform.SetRotation(0.0f, 0.0f, 0.0f);

		SetVertices();
		SetIndexBuffer();

	}
	WorldTrans& GetTransform()
	{
		return m_transform;
	}
	void SetPosition(float x, float y, float z)
	{
		m_transform.SetPosition(x, y, z);
	}
	void Rotate(float x, float y, float z)
	{
		m_transform.Rotate(x, y, z);
	}
	void AddShader(CompiledShaderProgram shader) {
		m_Shaders[m_ShaderIndex] = shader;
		m_ShaderIndex++;

		gWLocation = glGetUniformLocation(shader.ShaderProgram, "gWP");
		if (gWLocation == -1) {
			printf("Error getting uniform location 'gWP'\n");
			exit(1);
		}

		gViewLocation = glGetUniformLocation(shader.ShaderProgram, "gVP");
		if (gViewLocation == -1) {
			printf("Error getting uniform location 'gVP'\n");
			exit(1);
		}

		gSamplerLocation = glGetUniformLocation(shader.ShaderProgram, "gSampler");
		if (gSamplerLocation == -1) {
			printf("Warning, getting uniform location 'gSampler'\n");
			//	exit(1);
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

	//	glBindVertexArray(0);
	//	glDisableVertexAttribArray(0);
	//	glDisableVertexAttribArray(1);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
	}

	void SetVertices()
	{
		Vertices[0] = Vertex(0.5f, 0.5f, 0.5f,    0.5f, 0.0f, 0.0f, 1.0f, 1.0f);
		Vertices[1] = Vertex(-0.5f, 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f);
		Vertices[2] = Vertex(-0.5f, 0.5f, 0.5f,   0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertices[3] = Vertex(0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f);

		Vertices[4] = Vertex(-0.5f, -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertices[5] = Vertex(0.5f, 0.5f, -0.5f,   0.5f, 0.0f, 0.0f, 1.0f, 1.0f);
		Vertices[6] = Vertex(0.5f, -0.5f, 0.5f,   0.5f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertices[7] = Vertex(-0.5f, -0.5f, 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f);




		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

		// color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

		// texture
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));

		// View/World matrix
		glm::mat4x4 WorldTransform = m_transform.GetMatrix();
		glUniformMatrix4fv(gWLocation, 1, GL_FALSE, glm::value_ptr(WorldTransform));
	}
	void SetTexture(std::string path)
	{
		m_texture = std::make_unique<Texture>(path, texture);

		if (gSamplerLocation != -1) {
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(gSamplerLocation, 0);
		}
	}

	void Update(glm::mat4x4 &vp)
	{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glUseProgram(m_Shaders[0].ShaderProgram);

		if (gViewLocation != -1) {
			glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(vp));
		}
		if (gWLocation != -1) {
			glUniformMatrix4fv(gWLocation, 1, GL_FALSE, glm::value_ptr(GetTransform().GetMatrix()));
		}

		if (gSamplerLocation != -1) {
			glUniform1i(gSamplerLocation, 0);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		
		
		
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	}
private:

};

#endif 