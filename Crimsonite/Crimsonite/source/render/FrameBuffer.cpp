#include "FrameBuffer.h"

#include <gl/glew.h>

#include "../asset/AssetManager.h"

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferId);
}

void FrameBuffer::Resize(int _width, int _height)
{
	Bind();
	glViewport(0, 0, _width, _height);
	GetLinkedTexture()->SetSize(_width, _height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetLinkedTexture()->TextureId, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, DepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_width = _width;
	m_height = _height;
}
