#ifndef MODEL_OBJECT_H
#define MODEL_OBJECT_H

#include "Texture.h"
#include "Vertex.h"
#include "Base/Primitive.h"

#include <array>
#include <memory>


class ModelObject : public Primitive
{
private:
	std::unique_ptr<Texture> m_texture;
	
	Vertex Vertices[8];

	GLuint VBO = -1;
	GLuint IBO = -1;
	GLuint texture = -1;
	GLuint gSamplerLocation = -1;

	std::array<unsigned int, 36> m_indices;
public:
	ModelObject() : Primitive()
	{
		SetVertices();
		SetIndexBuffer();
	}
	virtual void AddShader(CompiledShaderProgram shader) override {
		Primitive::AddShader(shader);

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

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		
		if (gSamplerLocation != -1) {
			glUniform1i(gSamplerLocation, 0);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
private:

};

#endif 