#pragma once

class CameraSettings
{

public:
	
	//	The width of the camera view.
	int width = 1600;
	//	The height of the camera view.
	int height = 900;
	//	The field of view of the camera.
	float fov = 70.0f;
	//	The near-clip value of the camera.
	float nearClip = 0.001f;
	//	The far-clip value of the camera.
	float farClip = 5000.0f;
};