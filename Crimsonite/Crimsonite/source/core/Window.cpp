#include "Window.h"

#include "CrimsonCore.h"

Window * Window::m_instance(0);
Window * Window::Instance()
{
	if (!m_instance) m_instance = new Window();
	return m_instance;
}

void Window::SetFullscreen(bool _fullscreen)
{
	Instance()->m_core->SetWindowFullscreen(_fullscreen);
	Instance()->m_isFullScreen = _fullscreen;
}

void Window::SetCore(CrimsonCore * _core)
{
	Instance()->m_core = _core;
}

void Window::SetTransform(int _width, int _height, int _x, int _y)
{
	Instance()->m_windowTransform.width = _width;
	Instance()->m_windowTransform.height = _height;
	Instance()->m_windowTransform.posX = _x;
	Instance()->m_windowTransform.posY = _y;
}
