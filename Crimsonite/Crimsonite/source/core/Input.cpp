#include "Input.h"

#include <iostream>

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

void Input::SetKeyState(int _keycode, int _keyState)
{
	Instance()->m_keyArrayPrevFrame[_keycode] = Instance()->m_keyArray[_keycode];
	Instance()->m_keyArray[_keycode] = _keyState;
}
