#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include <SDL_opengl.h>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Texture
{
private:
	int width, height, nrChannels;
	std::string texturePath;
	unsigned char* data;
	GLuint texture;
	GLenum format;
	unsigned int textureIndex;
public:
	Texture(std::string texturePath, GLuint &outTexture, unsigned int texture_index = 0) : textureIndex(texture_index)
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
		
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		outTexture = texture;
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
