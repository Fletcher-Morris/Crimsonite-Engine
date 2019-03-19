#include "Time.h"

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
}
