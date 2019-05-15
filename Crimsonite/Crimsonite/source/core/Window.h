#pragma once

#include <iostream>

class CrimsonCore;

struct WindowTransform
{
	int width;
	int height;
	int posX;
	int posY;
};

class Window
{

public:
	
	static Window * Instance();

	static int Width() { return Instance()->m_width; }
	static int Height() { return Instance()->m_height; }
	static int RefreshRate() { return Instance()->m_refreshRate; }
	static bool IsFullscreen() { return Instance()->m_isFullScreen; }
	static WindowTransform Transform() { return Instance()->m_windowTransform; }

	static void UpdateGlfwMode(int _width, int _height, int _refresh)
	{
		Instance()->m_width = _width;
		Instance()->m_height = _height;
		Instance()->m_refreshRate = _refresh;
	}
	static void UpdateGlfwMode(int _width, int _height)
	{
		Instance()->m_width = _width;
		Instance()->m_height = _height;
	}
	static void SetFullscreen(bool _fullscreen);
	static void SetCore(CrimsonCore * _core);
	static void SetTransform(int _width, int _height, int _x, int _y);

private:

	int m_width;
	int m_height;
	int m_refreshRate;

	bool m_isFullScreen;
	WindowTransform m_windowTransform;

	CrimsonCore * m_core;

protected:

	static Window * m_instance;
};