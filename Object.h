#ifndef MODEL_OBJECT_H
#define MODEL_OBJECT_H

#include "Texture.h"
#include "Vertex.h"
#include "Base/Primitive.h"

#include <array>
#include <memory>
#include "Mesh/Basic/Cube.h"


class ModelObject : public Primitive
{
private:
	GLuint texture = -1;
	GLuint gSamplerLocation = -1;
	std::unique_ptr<Texture> m_texture;
	
public:
	ModelObject() : Primitive()
	{
		
	}
	virtual void AddShader(CompiledShaderProgram shader) override {
		Primitive::AddShader(shader);

		
	}


	void SetTexture(std::string sampler_name, std::string path)
	{
		m_texture = std::make_unique<Texture>(path, texture);

		SetUniform(sampler_name, &m_texture);

	//	if (gSamplerLocation == -1) {
	//		gSamplerLocation = glGetUniformLocation(m_Shaders[0].ShaderProgram, sampler_name.c_str());
	//	}
	//	
	//	if (gSamplerLocation == -1) {
	//		std::cout << "Warning, uniform sampler " << sampler_name << "not found \n";
	//		return;
	//	}
	//
	//	
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//	glUniform1i(gSamplerLocation, 0);
	}

	virtual void Update(glm::mat4x4 &vp) override
	{
		Primitive::Update(vp);
	}
private:

};

#endif 