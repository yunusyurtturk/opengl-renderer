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

		gSamplerLocation = glGetUniformLocation(shader.ShaderProgram, "gSampler");
		if (gSamplerLocation == -1) {
			printf("Warning, getting uniform location 'gSampler'\n");
			//	exit(1);
		}
	}


	void SetTexture(std::string path)
	{
		m_texture = std::make_unique<Texture>(path, texture);

		if (gSamplerLocation != -1) {
			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(gSamplerLocation, 0);
		}
	}

	virtual void Update(glm::mat4x4 &vp) override
	{
		Primitive::Update(vp);

		if (gSamplerLocation != -1) {
			glUniform1i(gSamplerLocation, 0);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
	}
private:

};

#endif 