#pragma once

#include <string>

class Texture;

class FrameBuffer
{

public:
	
	FrameBuffer() {};

	//	The ID of this FrameBuffer.
	unsigned int BufferId;
	//	Set the linked Texture.
	void SetTexture(Texture * _tex) { m_linkedTex = _tex; }

	//	The ID of the linked depth buffer.
	unsigned int DepthBufferId;
	//	Set the depth buffer ID.
	void SetDepthBuffer(unsigned int _id) { DepthBufferId = _id; };

	void Bind() {};
	void Unbind() {};

private:

	Texture * m_linkedTex;

};