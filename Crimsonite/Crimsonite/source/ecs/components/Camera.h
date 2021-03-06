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
	void SetCameraFov(float _fov);
	//	Set the near and farliiping values for this camera.
	void SetCameraClipping(float _near, float _far);
	//	Set the near and farliiping values for this camera.
	void SetCameraSettings(float _fov, float _near, float _far);
	//	Set thewidth and height of this camera.
	void SetCameraSize(int _width, int _height);
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

	//	Set the output FrameBuffer for this Camera.
	void SetOutputFrameBuffer(FrameBuffer * _buffer) { m_frameBuffer = _buffer; }
	//	Set the output FrameBuffer for this Camera by name.
	void SetOutputFrameBuffer(std::string _bufferName) { SetOutputFrameBuffer(AssetManager::GetFrameBuffer(_bufferName)); }
	//	Set the output FrameBuffer for this Camera by name, creating onf if it does not exist.
	void SetOutputFrameBuffer(std::string _bufferName, bool _create) { SetOutputFrameBuffer(AssetManager::GetFrameBuffer(_bufferName, _create)); }
	//	Resize this Camera's FrameBuffer.
	void ResizeFramebuffer(int _width, int _height);
	//	Return this Camera's FrameBuffer.
	FrameBuffer * GetOutputFrameBuffer() { return m_frameBuffer; }
	//	Return the name of this Camera' FrameBuffer, or 'NO_BUFFER' if it does not exist.
	std::string GetFrameBufferName() { if (m_frameBuffer) return GetOutputFrameBuffer()->GetName(); else return "NO_BUFFER"; }
	//	Force this camera to use a FrameBuffer.
	void EnforceFrameBuffer();

	//	Does this Camera's FrameBuffer automatically resize to match the window?
	bool AutoResize() { return m_autoResize; }
	//	Set whether this Camera's FrameBuffer automatically resizes to match the window.
	void SetAutoResize(bool _autoResize) { m_autoResize = _autoResize; }


private:

	//	The view setting used to render the camera.
	CameraSettings m_settings;

	//	The projection matrix of the camera.
	glm::mat4 m_projectionMatrix;
	//	The view matrix of the camera.
	glm::mat4 m_viewMatrix;
	//	The view-projection matrix of the camera.
	glm::mat4 m_projectionViewMatrix;

	//	The FrameBuffer used by this Camera.
	FrameBuffer * m_frameBuffer;

	//	The Renderer used by this Camera.
	Renderer * m_renderer;

	//	Does this Camera auto-resize?
	bool m_autoResize = true;

};