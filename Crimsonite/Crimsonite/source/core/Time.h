#pragma once

class Time
{

public:

	//	Return the singleton instance.
	static Time * Instance();

	//	Return DeltaTime as a float.
	static float DeltaTime() { return Instance()->m_deltaTime; }
	//	Return FixedTime as a float.
	static float FixedTime() { return Instance()->m_fixedTime; }
	//	Return DeltaTime as a double.
	static double DeltaTimeDouble() { return Instance()->m_deltaTimeDouble; }
	//	Return DeltaTime as a double.
	static double FixedTimeDouble() { return Instance()->m_fixedTimeDouble; }
	//	Return the current time as a float.
	static float GetTime() { return (float) Instance()->m_currentFrameTime; }
	//	Return the current time as a float.
	static double GetTimeDouble() { return Instance()->m_currentFrameTime; }

	//	Set the frame time to calculate DeltaTime.
	static void SetFrameTime(double _frameTime);

	static int GetFps() { return Instance()->m_averageFps; }

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

protected:

	//	The static singleton instance.
	static Time * m_instance;

};