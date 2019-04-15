#include "Input.h"

Input * Input::m_instance(0);
Input * Input::Instance()
{
	if (!m_instance) m_instance = new Input();
	return m_instance;
}
