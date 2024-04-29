#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include <SDL_opengl.h>
#include <GL/glew.h>

#include "stb_image.h"

class Texture
{
private:
	int width, height, nrChannels;
	std::string texturePath;
	
	unsigned char* data = 0;
	
	GLenum format;
	unsigned int textureIndex = 0;
	GLuint GenerateTexture(int width, int height)
	{
		glGenTextures(1, &texture);
		//		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); 
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		return texture;
	}
public:
	std::string typeName;
	GLuint texture;
	~Texture()
	{

	}
	Texture(Texture&& other)
	
		: width(other.width), height(other.height), nrChannels(other.nrChannels),
			texturePath(std::move(other.texturePath)), data(other.data), format(other.format) {
			// After moving, set the other's data pointer to nullptr to avoid double free
			other.data = nullptr;
	}
	Texture(std::string texture_path, std::string type_name = "") : textureIndex(0), texturePath(texture_path), typeName(type_name)
	{
		GLuint outTex = -1;
		Init(texturePath, outTex, textureIndex);
	}
	Texture(std::string texturePath, GLuint& outTexture, unsigned int texture_index = 0) : textureIndex(texture_index)
	{
		Init(texturePath, outTexture, texture_index);
	}
	Texture(GLenum tex_format, int in_width, int in_height, GLuint& outTexture) : width(in_width), height(in_height)
	{
		format = tex_format;
		Init(outTexture);
	}
	void Init(GLuint& outTexture)
	{
		outTexture = GenerateTexture(width, height);
	}
	void Init(std::string texturePath, GLuint& outTexture, unsigned int texture_index)
	{
		data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
		if (!data) {
			std::cout << "Failed to load texture";
			exit(1);
		}
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		outTexture = GenerateTexture(width, height);
		stbi_image_free(data);

		outTexture = texture;
	}

	const std::string& getPath()
	{
		return texturePath;
	}
	

	GLuint& getTexture()
	{
		return texture;
	}
	unsigned int& getTextureIndex()
	{
		return textureIndex;
	}

};

#endif
