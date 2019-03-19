#pragma once

#include <string>

class Texture
{

public:

	//	The ID of this Texture on the GPU.
	unsigned int TextureId;
	//	The ID of this Texture on the GPU.
	unsigned int ActiveSlot;

	//	The raw image data of the Texture.
	unsigned char* ImageData;

	Texture();
	Texture(std::string _name, int _width, int _height, unsigned char* _data);
	~Texture();

	//	Bind this Texture to the GPU.
	void Bind();
	void Bind(unsigned int _textureSlot);
	//	Unbind this Texture from the GPU.
	void Unbind();

	//	Set the width and heght of the Texture.
	void SetSize(int _width, int _height);
	//	Set the width of the Texture.
	void SetWidth(int _width);
	//	Set the height of the Texture.
	void SetHeight(int _height);
	//	Set the raw image data of the Texture.
	void SetData(unsigned char* _data);

	std::string GetName() { return m_name; }
	//	Return the width of the Texture.
	int GetWidth() { return m_width; }
	//	Return the height of the Texture.
	int GetHeight() { return m_height; }

private:

	//	The name of this Texture.
	std::string m_name;

	//	The width of this Texture in pixels.
	int m_width;
	//	The height of this Texture in pixels.
	int m_height;
};