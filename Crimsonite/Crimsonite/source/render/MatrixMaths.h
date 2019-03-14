#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>



class Transform;
class Camera;
class CameraSettings;

glm::mat4 CreateModelMatrix(const Transform & _transform);
glm::mat4 CreateViewMatrix(const Camera & _camera);
glm::mat4 CreateProjectionMatrix(const CameraSettings & _settings);