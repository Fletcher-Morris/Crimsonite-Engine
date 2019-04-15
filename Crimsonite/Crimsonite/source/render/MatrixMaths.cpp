#include "MatrixMaths.h"
#include "../ecs/Transform.h"
#include "../ecs/components/Camera.h"

glm::mat4 CreateModelMatrix(const Transform & _transform)
{
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, _transform.position);
	matrix = glm::rotate(matrix, glm::radians(_transform.rotation.x), { 1,0,0 });
	matrix = glm::rotate(matrix, glm::radians(_transform.rotation.y), { 0,1,0 });
	matrix = glm::rotate(matrix, glm::radians(_transform.rotation.z), { 0,0,1 });
	matrix = glm::scale(matrix, _transform.scale);
	return matrix;
}

glm::mat4 CreateViewMatrix(const Camera & _camera)
{
	return glm::lookAt(_camera.entity->transform.GetPosition(), _camera.entity->transform.GetPosition() + _camera.entity->transform.Forward(), _camera.entity->transform.Up());
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
