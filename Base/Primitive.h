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
#include "../Mesh/Mesh.h"


class Primitive
{
protected:
	GLuint gWLocation = -1;
	GLuint gViewLocation = -1;
	unsigned int m_ShaderIndex = 0;
	WorldTrans m_transform;
	std::array<CompiledShaderProgram, 8> m_Shaders;
	std::vector<std::shared_ptr<UniformSlot>> uniformElements;
	std::string m_Name;

	std::vector<std::unique_ptr<Mesh>> meshes;
public:
	Primitive()
	{
		m_transform.SetPosition(0.0f, 0.0f, 2.0f);
		m_transform.SetRotation(0.0f, 0.0f, 0.0f);
	}
	Primitive(const Primitive& rh)
	{

	}
	Primitive(Primitive&& source)
	{

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
			//exit(1);
		}

		gViewLocation = glGetUniformLocation(shader.ShaderProgram, "gVP");
		if (gViewLocation == -1) {
			std::cout << "Error getting uniform location 'gVP' in " << shader.FragmentShader << " or " << shader.VertexShader << "\n";
			//exit(1);
		}
	}
	template<typename T>
	void SetUniform(std::string uniform_name, T&& val) {
		
		using PType = typename std::remove_pointer<T>::type;

		glUseProgram(m_Shaders[0].ShaderProgram);

		if (m_Shaders[m_ShaderIndex - 1].ShaderProgram != -1) {
			std::shared_ptr<UniformElement<PType>> element = std::make_shared<UniformElement<PType>>(m_Shaders[m_ShaderIndex - 1].ShaderProgram, uniform_name);
			uniformElements.emplace_back(element);
			if constexpr (std::is_pointer_v<T>) {

				std::cout << "Binded as is_pointer_v" << "\n";
				element->bind(val);
				//static_cast<UniformElement<PType>*>(uniformElements.back().get())->bind(std::forward<T>(val));
			}
			else {
				//static_cast<UniformElement<PType>*>(uniformElements.back().get())->bind(std::forward<T>(val));
				std::cout << "Binded as is_pointer_v NOT" << "\n";
				element->bind(&val);
			}
		}
	}
	void AddMesh(std::unique_ptr<Mesh> p_model)
	{
		meshes.emplace_back(std::move(p_model));
	}
	void AddMesh(std::vector<std::unique_ptr<Mesh>>& meshes_vector)
	{
		meshes.insert(meshes.end(),
			std::make_move_iterator(meshes_vector.begin()),
			std::make_move_iterator(meshes_vector.end()));
		meshes_vector.clear();
	}
	void Setup()
	{
		for (auto& mesh : meshes)
			mesh->Setup();
	}
	virtual void Update(glm::mat4x4& vp)
	{
		Update(vp, m_Shaders[0]);
	}

	virtual void Update(glm::mat4x4& vp, CompiledShaderProgram& shader)
	{
		glUseProgram(shader.ShaderProgram);

		if (gViewLocation != -1) {
			glUniformMatrix4fv(gViewLocation, 1, GL_FALSE, glm::value_ptr(vp));
		}
		if (gWLocation != -1) {
			glUniformMatrix4fv(gWLocation, 1, GL_FALSE, glm::value_ptr(GetTransform().GetMatrix()));
		}

		for (auto& uniform : uniformElements) {
			uniform.get()->Update();
		}

		for (auto& mesh : meshes)
			mesh->Draw(m_Shaders[0]);
		
	}
};