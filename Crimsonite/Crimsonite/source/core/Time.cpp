#include "Time.h"

#include <iostream>

Time * Time::m_instance(0);
Time * Time::Instance()
{
	if (!m_instance) m_instance = new Time();
	return m_instance;
}

void Time::SetFrameTime(double _frameTime)
{
	Instance()->m_currentFrameTime = _frameTime;
	m_instance->m_deltaTimeDouble = m_instance->m_currentFrameTime - m_instance->m_lastFrameTime;
	m_instance->m_lastFrameTime = m_instance->m_currentFrameTime;
	m_instance->m_deltaTime = (float)m_instance->m_deltaTimeDouble;
	m_instance->m_frameCounter++;
	if (_frameTime >= m_instance->m_lastFpsCheckTime + 1.0f)
	{
		m_instance->m_averageFps = m_instance->m_frameCounter;
		m_instance->m_frameCounter = 0;
		m_instance->m_lastFpsCheckTime = _frameTime;
	}
}
