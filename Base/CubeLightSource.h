#pragma once
#include <array>
#include "Primitive.h"
#include "../Vertex.h"

class CubeLightSource : public Primitive
{
private:


	GLuint VBO = -1;
	GLuint IBO = -1;
	GLuint objectColorLocation = -1;
	GLuint lightColorLocation = -1;
	glm::vec3 objectColor = glm::vec3(1.0f, 1.5f, 0.5f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

public:
	CubeLightSource() : Primitive()
	{
		
	}
	virtual void AddShader(CompiledShaderProgram shader) {

		Primitive::AddShader(shader);
	}

	glm::vec3& GetLightColorRef()
	{
		return lightColor;
	}
	glm::vec3& GetObjectColorRef()
	{
		return objectColor;
	}

	virtual void Update(glm::mat4x4& vp) override
	{
		Primitive::Update(vp);
	}
};