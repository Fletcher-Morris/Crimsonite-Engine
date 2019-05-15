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

	void SetName(std::string _name) { m_name = _name; }
	std::string GetName() { return m_name; }

	void Bind();
	void Unbind() {};

	void Resize(int _width, int _height);
	int Width() { return m_width; }
	int Height() { return m_height; }

private:

	Texture * m_linkedTex;
	std::string m_name;
	int m_width;
	int m_height;

};