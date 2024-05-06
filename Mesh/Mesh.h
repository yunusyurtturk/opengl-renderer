#pragma once
#include <vector>
#include "../Vertex.h"
#include "../Texture.h"

class Texture;

class Mesh
{
private:
	GLuint VBO = -1;
	GLuint IBO = -1;

	std::vector<Vertex> vertex;
	std::vector<int> index;
	std::vector<Texture *> textures;
protected:
	int vertexCount = 0;
	int indexCount = 0;
	GLuint VAO = -1;
public:
	Mesh()
	{

	}
	Mesh(Mesh&& source)
	{
		vertex = source.vertex;
		index = source.index;
		textures = source.textures;

	}
	Mesh(const Mesh& source)
	{

	}
	std::vector<Vertex>& GetVertex()
	{
		return vertex;
	}
	std::vector<int>& GetIndex()
	{
		return index;
	}
	std::vector<Texture *>& GetTextures()
	{
		return textures;
	}

	void AddTexture(Texture * texture)
	{
		textures.emplace_back(texture);
	}

	void AddVertex(Vertex&& new_vertex)
	{
		vertex.push_back(new_vertex);
	}
	void AddIndex(int&& new_index)
	{
		index.push_back(new_index);
	}
	virtual void Setup()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		initVertex();
		initIndex();
		glBindVertexArray(0);
	}
	void initVertex()
	{
		std::vector<Vertex>& Vertices = GetVertex();
		vertexCount = Vertices.size();

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

		// position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		// normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6 * sizeof(float)));
		// Texture
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9 * sizeof(float)));

	}
	void initIndex()
	{
		std::vector<int>& Index = GetIndex();

		if (Index.size() > 0) {
			indexCount = Index.size();
			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int), &Index[0], GL_STATIC_DRAW); 
		}
	}
	virtual void Draw(CompiledShaderProgram &shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			std::string number;
			std::string name = textures[i]->typeName;
			if (name == "texture_diffuse") {
				number = std::to_string(diffuseNr++);
			}
			else if (name == "texture_specular") {
				number = std::to_string(specularNr++);
			}
			else if (name == "texture_normal") {
				number = std::to_string(normalNr++);
			}
			else if (name == "texture_height") {
				number = std::to_string(heightNr++);
			}
			glUniform1i(glGetUniformLocation(shader.ShaderProgram, (name + number).c_str()), i);
			// and finally bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i]->texture);
		}
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0);
	}
};