#pragma once

#include <SOIL2/SOIL2.h>

class Texture
{

public:

	//	The ID of this Texture on the GPU.
	unsigned int TextureId;

	Texture();
	
	//	Bind this Texture to the GPU.
	void Bind();
	//	Unbind this Texture from the GPU.
	void Unbind();

private:

	//	The width of this Texture in pixels.
	int m_width;
	//	The height of this Texture in pixels.
	int m_height;

};