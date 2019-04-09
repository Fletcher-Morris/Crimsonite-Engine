#pragma once

#include "../ECS.h"

#include <glm/common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "../../render/CameraSettings.h"

class Renderer;

class Camera : public EcsComponent
{

public:

	virtual void OnInit() override;
	virtual void OnUpdate() override;
	virtual void OnFixedUpdate() override;
	virtual void OnRender() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void DrawEditorProperties() override;
	virtual std::string Serialize() override;
	virtual void Deserialize(std::vector<std::string> _data) override;

	//	Set the view settings for this camera.
	void SetCameraSettings(CameraSettings _newSettings);
	//	Set the feld of view of this camera.
	void SetCameraSettings(float _fov);
	//	Set the near and farliiping values for this camera.
	void SetCameraSettings(float _near, float _far);
	//	Set the near and farliiping values for this camera.
	void SetCameraSettings(float _fov, float _near, float _far);
	//	Set thewidth and height of this camera.
	void SetCameraSettings(int _width, int _height);
	//	Return the view settingsused for this camera.
	CameraSettings GetCameraSettings() { return m_settings; }

	//	Set the renderer reference.
	void SetRenderer(Renderer * _renderer) { m_renderer = _renderer; }

	//	Bind this Camera.
	void Bind();

	//	Initialize the camera using it's settings.
	void ReInit();
	//	Get the projection matrix for this camera.
	const glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }
	//	Get the view matrix for this camera.
	const glm::mat4 GetViewMatrix() { return m_viewMatrix; }
	void CreateCameraViewMatrix();
	//	Get the view-projection matrix for this camera.
	const glm::mat4 GetProjectionViewMatrix() { return m_projectionViewMatrix; }

	void SetOutputFrameBuffer(FrameBuffer * _buffer) { m_frameBuffer = _buffer; }
	void SetOutputFrameBuffer(std::string _bufferName) { SetOutputFrameBuffer(AssetManager::Instance()->GetFrameBuffer(_bufferName)); }
	void UpdateOutputBufferSize();
	FrameBuffer * GetOutputFrameBuffer() { return m_frameBuffer; }


private:

	//	The view setting used to render the camera.
	CameraSettings m_settings;

	//	The projection matrix of the camera.
	glm::mat4 m_projectionMatrix;
	//	The view matrix of the camera.
	glm::mat4 m_viewMatrix;
	//	The view-projection matrix of the camera.
	glm::mat4 m_projectionViewMatrix;

	FrameBuffer * m_frameBuffer;

	Renderer * m_renderer;

};