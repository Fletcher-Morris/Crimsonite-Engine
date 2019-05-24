#pragma once

class Time
{

public:

	//	Return the singleton instance.
	static Time * Instance();

	//	Return DeltaTime as a float.
	static float DeltaTime();
	//	Return DeltaTime as a double.
	static double DeltaTimeDouble();

	//	Return the current time as a float.
	static float GetTime();
	//	Return the current time as a float.
	static double GetTimeDouble();
	//	Return the current scaled time as a float.
	static float GetScaledTime();
	//	Return the current scaled time as a float.
	static double GetScaledTimeDouble();

	//	Set the frame time to calculate DeltaTime.
	static void SetFrameTime(double _frameTime);

	//	Return the average FPS over the past second.
	static int GetFps();

	//	Set the time scale factor as a float.
	static void SetTimeScale(float m_newScale);
	//	Set the time scale factor as a double.
	static void SetTimeScale(double m_newScale);
	//	Return the time scaling factor as a float.
	static float GetTimeScale();
	//	Return the time scaling factor as a double.
	static double GetTimeScaleDouble();

private:

	//	The time since the last frame as a float.
	float m_deltaTime;
	//	The time since the last fixed frame as a float.
	float m_fixedTime;
	//	The time since the last frame as a double.
	double m_deltaTimeDouble;
	//	The time since the last fixed frame as a double.
	double m_fixedTimeDouble;

	//	The time the current frame started.
	double m_currentFrameTime;
	//	The time the previous frame started.
	double m_lastFrameTime;

	//	A frame counter for measuring FPS.
	int m_frameCounter = 0;
	double m_lastFpsCheckTime;
	int m_averageFps = 0;

	//	The time scale multiplier as a float.
	float m_timeScale = 1.0f;
	//	The time scale multiplier as a double.
	double m_timeScaleDouble = 1.0;


protected:

	//	The static singleton instance.
	static Time * m_instance;

};