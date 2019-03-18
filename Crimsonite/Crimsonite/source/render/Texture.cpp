#include "Texture.h"

#include <gl/glew.h>

Texture::Texture()
{
}

Texture::Texture(int _width, int _height, unsigned char * _data)
{
	SetSize(_width, _height);
	SetData(_data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, TextureId);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void Texture::SetWidth(int _width)
{
	m_width = _width;
}

void Texture::SetHeight(int _height)
{
	m_height = _height;
}

void Texture::SetData(unsigned char * _data)
{
	ImageData = _data;
}
