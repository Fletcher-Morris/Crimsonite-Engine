#include "Time.h"

Time * Time::m_instance(0);
Time * Time::Instance()
{
	if (!m_instance) m_instance = new Time();
	return m_instance;
}

float Time::DeltaTime()
{
	return Instance()->m_deltaTime * m_instance->m_timeScale;
	return 0.0f;
}

double Time::DeltaTimeDouble()
{
	return Instance()->m_deltaTimeDouble * m_instance->m_timeScaleDouble;
}

float Time::GetTime()
{
	return (float)Instance()->m_currentFrameTime;
}

double Time::GetTimeDouble()
{
	return Instance()->m_currentFrameTime;
}

float Time::GetScaledTime()
{
	return ((float)Instance()->m_currentFrameTime) * m_instance->m_timeScale;
}

double Time::GetScaledTimeDouble()
{
	return Instance()->m_currentFrameTime * m_instance->m_timeScaleDouble;
}

void Time::SetFrameTime(double _frameTime)
{
	Instance()->m_currentFrameTime = _frameTime;
	m_instance->m_deltaTimeDouble = m_instance->m_currentFrameTime - m_instance->m_lastFrameTime;
	m_instance->m_lastFrameTime = m_instance->m_currentFrameTime;
	m_instance->m_deltaTime = (float) m_instance->m_deltaTimeDouble;
	m_instance->m_frameCounter++;
	if (_frameTime >= m_instance->m_lastFpsCheckTime + 1.0f)
	{
		m_instance->m_averageFps = m_instance->m_frameCounter;
		m_instance->m_frameCounter = 0;
		m_instance->m_lastFpsCheckTime = _frameTime;
	}
}

int Time::GetFps()
{
	return Instance()->m_averageFps;
}

void Time::SetTimeScale(float _newScale)
{
	Instance()->m_timeScale = _newScale;
	m_instance->m_timeScaleDouble = (double)_newScale;
}

void Time::SetTimeScale(double _newScale)
{
	Instance()->m_timeScaleDouble = _newScale;
	m_instance->m_timeScale = (float) _newScale;
}

float Time::GetTimeScale()
{
	return Instance()->m_timeScale;
}

double Time::GetTimeScaleDouble()
{
	return Instance()->m_timeScaleDouble;
}
