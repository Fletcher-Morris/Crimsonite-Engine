#include "Time.h"

Time * Time::m_instance(0);
Time * Time::Instance()
{
	if (!m_instance) m_instance = new Time();
	return m_instance;
}