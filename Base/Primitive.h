#pragma once

#include <iostream>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <array>
#include <unordered_map>

#include "../WorldTransform.h"
#include "../CompiledShaderProgram.h"


class Primitive
{
protected:
	GLuint VAO = -1;
	WorldTrans m_transform;
	unsigned int m_ShaderIndex = 0;
	std::array<CompiledShaderProgram, 8> m_Shaders;
	std::string m_Name;

	GLuint gWLocation = -1;
	GLuint gViewLocation = -1;

	std::unordered_map<std::string, std::pair<GLuint, void*>> m_UniformLocations;

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
	const std::string& GetName() const
	{
		return m_Name;
	}
	void SetName(const std::string &name)
	{
		m_Name = name;
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
			std::cout << "Error getting uniform location 'gWP' in " << shader.FragmentShader << " or " << shader.VertexShader << "\n";
			exit(1);
		}

		gViewLocation = glGetUniformLocation(shader.ShaderProgram, "gVP");
		if (gViewLocation == -1) {
			std::cout << "Error getting uniform location 'gVP' in " << shader.FragmentShader << " or " << shader.VertexShader << "\n";
			exit(1);
		}
	}
	
	virtual void SetUniform1f(std::string uniform_name, float val)
	{
		GLuint uniform_location = glGetUniformLocation(m_Shaders[m_ShaderIndex].ShaderProgram, uniform_name.c_str());
		if (uniform_location == -1) {
			std::cout << "Error getting uniform location " << uniform_name << " in " << m_Shaders[m_ShaderIndex].FragmentShader << " or " << m_Shaders[m_ShaderIndex].VertexShader << "\n";
			return;
		}
		m_UniformLocations[uniform_name] = std::make_pair(uniform_location, &val);
	}
	virtual void SetUniform3fv(std::string uniform_name, glm::vec3* vec3)
	{
		GLuint uniform_location = glGetUniformLocation(m_Shaders[m_ShaderIndex-1].ShaderProgram, uniform_name.c_str());
		if (uniform_location == -1) {
			std::cout << "Error getting uniform location " << uniform_name << " in " << m_Shaders[m_ShaderIndex].FragmentShader << " or " << m_Shaders[m_ShaderIndex].VertexShader << "\n";
			return;
		}
		m_UniformLocations[uniform_name] = std::make_pair(uniform_location, vec3);
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



		for (auto& uniform : m_UniformLocations) {
			glUniform3fv(uniform.second.first, 1, glm::value_ptr(*(glm::vec3 *)(uniform.second.second)));
		}
	}
};