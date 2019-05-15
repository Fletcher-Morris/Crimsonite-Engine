#include "MatrixMaths.h"
#include "../ecs/Transform.h"
#include "../ecs/components/Camera.h"

glm::mat4 CreateModelMatrix(Transform * _transform)
{
	glm::mat4 matrix = glm::mat4(1.0f);
	glm::vec3 rot = _transform->GetWorldRotation();
	matrix = glm::translate(matrix, _transform->GetWorldPosition());
	matrix = glm::rotate(matrix, glm::radians(rot.x), { 1,0,0 });
	matrix = glm::rotate(matrix, glm::radians(rot.y), { 0,1,0 });
	matrix = glm::rotate(matrix, glm::radians(rot.z), { 0,0,1 });
	matrix = glm::scale(matrix, _transform->GetScale());
	return matrix;
}

glm::mat4 CreateViewMatrix(const Camera & _camera)
{
	return glm::lookAt(_camera.entity->transform.GetWorldPosition(), _camera.entity->transform.GetWorldPosition() + _camera.entity->transform.Forward(), -_camera.entity->transform.Up());
}

glm::mat4 CreateProjectionMatrix(const CameraSettings & _settings)
{
	float safeWidth = _settings.width;
	float safeHeight = _settings.height;
	if (safeHeight == 0.0f)
	{
		safeWidth = 1.0f;
		safeHeight = 1.0f;
	}
	return CreateProjectionMatrix(_settings.fov, (float)(safeWidth / safeHeight), _settings.nearClip, _settings.farClip);
}

glm::mat4 CreateProjectionMatrix(const float _fov, const float _ratio, const float _near, const float _far)
{
	return glm::perspective(glm::radians(_fov), _ratio, _near, _far);
}
