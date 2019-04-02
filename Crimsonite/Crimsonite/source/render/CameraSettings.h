#pragma once

class CameraSettings
{

public:
	
	//	The width of the camera view.
	int width = 265;
	//	The height of the camera view.
	int height = 256;
	//	The field of view of the camera.
	float fov = 60.0f;
	//	The near-clip value of the camera.
	float nearClip = 0.1f;
	//	The far-clip value of the camera.
	float farClip = 100.0f;

	int value = 0;
	void UpdateValue()
	{
		value = 0;
		value += (width * 1);
		value += (height * 1000);
		value += (fov * 1000000);
		value += (nearClip * 1000000000);
		value += (farClip * 1000000000000);
	}
};