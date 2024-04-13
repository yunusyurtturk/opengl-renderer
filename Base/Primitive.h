#pragma once

#include <iostream>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 
#include <array>
#include <vector>
#include <unordered_map>
#include <memory>

#include "../WorldTransform.h"
#include "../CompiledShaderProgram.h"
#include "../Mesh/Model.h"


class Primitive
{
protected:
	GLuint VAO = -1;
	GLuint gWLocation = -1;
	GLuint gViewLocation = -1;
	unsigned int m_ShaderIndex = 0;
	WorldTrans m_transform;
	std::array<CompiledShaderProgram, 8> m_Shaders;
	std::vector<std::unique_ptr<UniformSlot>> uniformElements;
	std::string m_Name;

	std::unique_ptr<Model> model;
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
	template<typename T>
	void SetUniform(std::string uniform_name, T&& val) {
		
		using PType = typename std::remove_pointer<T>::type;

		if (m_Shaders[m_ShaderIndex - 1].ShaderProgram != -1) {
			
			uniformElements.emplace_back(std::make_unique<UniformElement<PType>>(m_Shaders[m_ShaderIndex - 1].ShaderProgram, uniform_name));
			if constexpr (std::is_pointer_v<T>) {
				static_cast<UniformElement<PType>*>(uniformElements.back().get())->bind(val);
			}
			else {
				static_cast<UniformElement<PType>*>(uniformElements.back().get())->bind(&val);
			}
		}
	}
	void SetModel(std::unique_ptr<Model> p_model)
	{
		model = std::move(p_model);
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


		for (auto& uniform : uniformElements) {
			uniform.get()->Update();
		}

		if (model)
			model->Draw();
	}
};