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
	std::vector<GLuint> texture;
	GLuint gSamplerLocation = -1;
	std::vector<Texture *> m_textures;
	unsigned int m_textureIndex = 0;
	
public:
	ModelObject() : Primitive()
	{
		texture.resize(8);
		m_textures.resize(8);
	}
	ModelObject(const ModelObject& rh)
	{

	}
	ModelObject(ModelObject&& source)
	{

	}
	
	virtual void AddShader(CompiledShaderProgram shader) override {
		Primitive::AddShader(shader);
	}

	void SetTexture(std::string sampler_name, std::string path)
	{
		texture.push_back(0);
		Texture *text = new Texture(path, texture.back(), m_textureIndex);
		m_textures.push_back(text);
		SetUniform(sampler_name, &text);
		m_textureIndex++;

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