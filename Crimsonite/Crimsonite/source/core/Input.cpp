#include "Input.h"

#include <iostream>
#include "glfw3.h"

Input * Input::m_instance(0);
Input * Input::Instance()
{
	if (!m_instance) m_instance = new Input();
	return m_instance;
}

bool Input::GetKey(int _keycode)
{
	if (Instance()->m_keyArray[_keycode] == KEYSTATE_HELD || Instance()->m_keyArray[_keycode] == KEYSTATE_PRESSED) return true;
	else return false;
}

bool Input::GetKeyDown(int _keycode)
{
	if (Instance()->m_keyArray[_keycode] == KEYSTATE_PRESSED) return true;
	else return false;
}

bool Input::GetKeyUp(int _keycode)
{
	if (Instance()->m_keyArray[_keycode] == KEYSTATE_RELEASED)
	{
		if (Instance()->m_keyArrayPrevFrame[_keycode] != KEYSTATE_RELEASED)
		{
			return true;
		}
	}
	return false;
}

glm::vec2 Input::GetMousePos()
{
	return Instance()->m_mousePos;
}

glm::vec2 Input::GetMouseMovement()
{
	return GetMouseMovement(0.0f);
}

glm::vec2 Input::GetMouseMovement(float _min)
{
	glm::vec2 movement = Instance()->m_mousePos - m_instance->m_mousePosPrevFrame;
	float length = glm::length(movement);
	if (length >= _min) return movement;
	else return glm::vec2();
}

void Input::SetKeyState(int _keycode, int _keyState)
{
	Instance()->m_keyArrayPrevFrame[_keycode] = Instance()->m_keyArray[_keycode];
	Instance()->m_keyArray[_keycode] = _keyState;
}

void Input::SetMousePos(double _x, double _y)
{
	Instance()->m_mousePosPrevFrame = Instance()->m_mousePos;
	m_instance->m_mousePos.x = (float)_x;
	m_instance->m_mousePos.y = (float)_y;
}

std::string Input::KeycodeToName(int _keycode)
{
	return (std::string) glfwGetKeyName(_keycode, 0);
}
