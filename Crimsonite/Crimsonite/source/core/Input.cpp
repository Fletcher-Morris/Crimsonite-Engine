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
	if (Instance()->m_keyArray[_keycode] == INPUTSTATE_HELD) return true;
	else if (Instance()->m_keyArray[_keycode] == INPUTSTATE_PRESS) return true;
	return false;
}

bool Input::GetKeyDown(int _keycode)
{
	if (Instance()->m_keyArray[_keycode] == INPUTSTATE_PRESS) return true;
	else return false;
}

bool Input::GetKeyUp(int _keycode)
{
	if (Instance()->m_keyArray[_keycode] == INPUTSTATE_RELEASE)
	{
		if (Instance()->m_keyArrayPrev[_keycode] != INPUTSTATE_RELEASE)
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
	glm::vec2 movement = Instance()->m_mousePos - m_instance->m_mousePosPrev;
	float length = glm::length(movement);
	if (length >= _min) return movement;
	else return glm::vec2();
}

bool Input::GetMouseButton(int _button)
{
	if (Instance()->m_mouseButtonArray[_button] == INPUTSTATE_HELD) return true;
	else if (Instance()->m_mouseButtonArray[_button] == INPUTSTATE_PRESS) return true;
	return false;
}

bool Input::GetMouseButtonDown(int _button)
{
	if (Instance()->m_mouseButtonArray[_button] == INPUTSTATE_PRESS) return true;
	else return false;
}

bool Input::GetMouseButtonUp(int _button)
{
	if (Instance()->m_mouseButtonArray[_button] == INPUTSTATE_RELEASE)
	{
		if (Instance()->m_mouseButtonArray[_button] != INPUTSTATE_RELEASE)
		{
			return true;
		}
	}
	return false;
}

void Input::SetKeyState(int _keycode, int _keyState)
{
	Instance()->m_keyArrayPrev[_keycode] = Instance()->m_keyArray[_keycode];
	Instance()->m_keyArray[_keycode] = _keyState;
}

void Input::SetMousePos(double _x, double _y)
{
	Instance()->m_mousePosPrev = Instance()->m_mousePos;
	m_instance->m_mousePos.x = (float)_x;
	m_instance->m_mousePos.y = (float)_y;
}

void Input::SetMouseButton(int _button, int _state)
{
	Instance()->m_mouseButtonArrayPrev[_button] = Instance()->m_mouseButtonArray[_button];
	m_instance->m_mouseButtonArray[_button] = _state;
}

std::string Input::KeycodeToName(int _keycode)
{
	return (std::string) glfwGetKeyName(_keycode, 0);
}