#pragma once
#include <GL/glew.h>
#include "../Texture.h"


class Framebuffer
{
private:
	GLuint FBO = -1;
	GLuint RBO = -1;
	GLuint textureID = -1;
	bool isInited = false;
	int width;
	int height;
	std::shared_ptr<Texture> texture;
	void InitTexture()
	{
		texture = std::make_shared<Texture>(GL_RGB, width, height, textureID);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
	}
public:
	Framebuffer(int in_width, int in_height)
	{
		width = in_width;
		height = in_height;
	}
	~Framebuffer()
	{

	}
	std::shared_ptr<Texture> GetTexture() const
	{
		return texture;
	}

	void Setup()
	{
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		InitTexture();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			volatile int a = 1;
			//InitTexture();
		}

		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	bool Attach()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glEnable(GL_DEPTH_TEST);
		return true;
	}
	bool Detach()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return true;
	}
	bool Delete()
	{
		glDeleteFramebuffers(1, &FBO);
		return true;
	}
};