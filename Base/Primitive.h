#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <array>
#include "../WorldTransform.h"
#include "../CompiledShaderProgram.h"

class Primitive
{
protected:
	GLuint VAO = -1;
	WorldTrans m_transform;
	unsigned int m_ShaderIndex = 0;
	std::array<CompiledShaderProgram, 8> m_Shaders;

	GLuint gWLocation = -1;
	GLuint gViewLocation = -1;

public:
	Primitive()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		m_transform.SetPosition(0.0f, 0.0f, 2.0f);
		m_transform.SetRotation(0.0f, 0.0f, 0.0f);

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
	virtual void AddShader(CompiledShaderProgram shader) {
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
	}
	virtual void Update(glm::mat4x4& vp)
	{
		glBindVertexArray(VAO);
		glUseProgram(m_Shaders[0].ShaderProgram);

		if (gViewLocation != -1) {
			glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(vp));
		}
		if (gWLocation != -1) {
			glUniformMatrix4fv(gWLocation, 1, GL_FALSE, glm::value_ptr(GetTransform().GetMatrix()));
		}
	}
};