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

public:
	Texture(std::string texturePath, GLuint &outTexture)
	{
		data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
		if (!data) {
			std::cout << "Failed to load texture";
			exit(1);
		}

		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		outTexture = texture;
	}

	GLuint& getTexture()
	{
		return texture;
	}

};

#endif
