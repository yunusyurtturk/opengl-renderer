#pragma once
#include <GL/glew.h>
#include "../Texture.h"


class Renderbuffer
{
private:
	GLuint RBO = -1;
	GLuint textureID = -1;
	bool isInited = false;
	int width;
	int height;
	std::shared_ptr<Texture> texture;
	void InitTexture()
	{
		texture = std::make_shared<Texture>(GL_RGB, width, height, textureID);
		glRenderbufferTexture2D(GL_Renderbuffer, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);
	}
public:
	Renderbuffer(int in_width, int in_height)
	{
		width = in_width;
		height = in_height;
	}
	~Renderbuffer()
	{

	}
	std::shared_ptr<Texture> GetTexture() const
	{
		return texture;
	}

	void Setup()
	{
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	bool Attach()
	{
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glEnable(GL_DEPTH_TEST);
		return true;
	}
	bool Detach()
	{
		glBindRenderbuffer(GL_Renderbuffer, 0);
		return true;
	}
	bool Delete()
	{
		glDeleteRenderbuffers(1, &RBO);
		return true;
	}
};