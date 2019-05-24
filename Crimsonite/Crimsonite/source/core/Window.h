#pragma once

#include <iostream>

class CrimsonCore;

//	A struct representing the transformation of the window.
struct WindowTransform
{
	//	The width of the window.
	int width;
	//	The height of the window.
	int height;
	//	The X position of the window.
	int posX;
	//	The Y position of the window.
	int posY;
};

class Window
{

public:
	
	//	The singleton instance of the Window class.
	static Window * Instance();

	//	Return the width of the widow.
	static int Width() { return Instance()->m_windowTransform.width; }
	//	Return the height of the widow.
	static int Height() { return Instance()->m_windowTransform.height; }
	//	Return the refresh rate of the widow.
	static int RefreshRate() { return Instance()->m_refreshRate; }
	//	Is the window full-screen?
	static bool IsFullscreen() { return Instance()->m_isFullScreen; }
	//	Return thetransform of the window.
	static WindowTransform Transform() { return Instance()->m_windowTransform; }

	//	Update the width, height, and refersh rate of the window.
	static void UpdateWindowStats(int _width, int _height, int _refresh)
	{
		Instance()->m_windowTransform.width = _width;
		Instance()->m_windowTransform.height = _height;
		Instance()->m_refreshRate = _refresh;
	}
	//	Update the width, and height of the window.
	static void UpdateWindowStats(int _width, int _height)
	{
		Instance()->m_windowTransform.width = _width;
		Instance()->m_windowTransform.height = _height;
	}
	//	Set the fullscreen state of the widow.
	static void SetFullscreen(bool _fullscreen);
	//	Set the engine core reference.
	static void SetCore(CrimsonCore * _core);
	//	Set the transform values of the window.
	static void SetTransform(int _width, int _height, int _x, int _y);

private:

	//	Is the window full-screen?
	bool m_isFullScreen;
	//	The transform of the window.
	WindowTransform m_windowTransform;
	//	The refresh rate of the window.
	int m_refreshRate;

	//	A reference to the engine core.
	CrimsonCore * m_core;

protected:

	//	A reference to the static instance of the class.
	static Window * m_instance;
};