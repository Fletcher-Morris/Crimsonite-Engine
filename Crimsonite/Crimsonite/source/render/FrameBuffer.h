#pragma once

#include <string>

class Texture;

class FrameBuffer
{

public:
	
	FrameBuffer() {};

	//	The ID of this FrameBuffer.
	unsigned int FrameBufferId = 0;

	//	Set the linked Texture.
	void SetTexture(Texture * _tex) { m_linkedTex = _tex; }
	Texture * GetLinkedTexture() { return m_linkedTex; }

	//	The ID of the linked depth buffer.
	unsigned int DepthBufferId;

	void Bind();
	void Unbind() {};

private:

	Texture * m_linkedTex;

};