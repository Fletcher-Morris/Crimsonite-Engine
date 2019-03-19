#include "MatrixMaths.h"
#include "../ecs/Transform.h"
#include "../ecs/components/Camera.h"

glm::mat4 CreateModelMatrix(const Transform & _transform)
{
	glm::mat4 matrix;
	matrix = glm::rotate(matrix, glm::radians(_transform.rotation.x), { 1,0,0 });
	matrix = glm::rotate(matrix, glm::radians(_transform.rotation.y), { 0,1,0 });
	matrix = glm::rotate(matrix, glm::radians(_transform.rotation.z), { 0,0,1 });
	return matrix;
}

glm::mat4 CreateViewMatrix(const Camera & _camera)
{
	glm::mat4 matrix;
	matrix = glm::rotate(matrix, glm::radians(_camera.entity->transform.rotation.x), { 1,0,0 });
	matrix = glm::rotate(matrix, glm::radians(_camera.entity->transform.rotation.y), { 0,1,0 });
	matrix = glm::rotate(matrix, glm::radians(_camera.entity->transform.rotation.z), { 0,0,1 });
	matrix = glm::translate(matrix, -_camera.entity->transform.position);
	return matrix;
}

glm::mat4 CreateProjectionMatrix(const CameraSettings & _settings)
{
	glm::mat4 matrix;
	matrix = glm::perspective(glm::radians(_settings.fov), (float)(_settings.width / _settings.height), _settings.nearClip, _settings.farClip);
	return matrix;
}
