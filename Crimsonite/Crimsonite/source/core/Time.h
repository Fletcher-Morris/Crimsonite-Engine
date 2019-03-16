#pragma once

class Time
{

public:

	//	Return the singleton instance.
	static Time * Instance();

	//	Return DeltaTime as a float.
	float DeltaTime() { return m_deltaTime; }
	//	Return FixedTime as a float.
	float FixedTime() { return m_fixedTime; }
	//	Return DeltaTime as a double.
	double DeltaTimeDouble() { return m_deltaTimeDouble; }
	//	Return DeltaTime as a double.
	double FixedTimeDouble() { return m_fixedTimeDouble; }

	//	Set DeltaTime as a float.
	void SetDeltaTime(float _dt) { m_deltaTime = _dt; }
	//	Set FixedTime as a float.
	void SetFixedTime(float _ft) { m_fixedTime = _ft; }
	//	Set DeltaTime as a double.
	void SetDeltaTimeDouble(double _dt) { m_deltaTimeDouble = _dt; }
	//	Set FixedTime as a double.
	void SetFixedTimeDouble(double _ft) { m_fixedTimeDouble = _ft; }

private:

	//	The time since the last frame as a float.
	float m_deltaTime;
	//	The time since the last fixed frame as a float.
	float m_fixedTime;
	//	The time since the last frame as a double.
	double m_deltaTimeDouble;
	//	The time since the last fixed frame as a double.
	double m_fixedTimeDouble;

protected:

	//	The static singleton instance.
	static Time * m_instance;

};