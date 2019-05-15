#include "Texture.h"

#include <gl/glew.h>

Texture::Texture()
{
}

Texture::Texture(std::string _name, int _width, int _height, unsigned char * _data)
{
	m_name = _name;
	SetSize(_width, _height);
	SetData(_data);
}

Texture::Texture(std::string _name, int _width, int _height)
{
	m_name = _name;
	SetSize(_width, _height);
}

Texture::~Texture()
{
	glDeleteTextures(1, &TextureId);
}

void Texture::Bind()
{
	Bind(0);
}

void Texture::Bind(unsigned int _textureSlot)
{
	ActiveSlot = (GL_TEXTURE0 + _textureSlot);
	glActiveTexture(ActiveSlot);
	glBindTexture(GL_TEXTURE_2D, TextureId);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetSize(int _width, int _height)
{
	Bind();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	m_width = _width;
	m_height = _height;
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
