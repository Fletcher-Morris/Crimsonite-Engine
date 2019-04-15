#pragma once

#include <iostream>

class Window
{

public:
	
	static Window * Instance();

	static int Width() { return Instance()->m_width; }
	static int Height() { return Instance()->m_height; }

	static void SetSize(int _width, int _height)
	{
		Instance()->m_width = _width;
		Instance()->m_height = _height;
	}


private:

	int m_width;
	int m_height;

protected:

	static Window * m_instance;
};