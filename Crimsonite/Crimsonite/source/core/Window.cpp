#include "Window.h"

Window * Window::m_instance(0);
Window * Window::Instance()
{
	if (!m_instance) m_instance = new Window();
	return m_instance;
}
