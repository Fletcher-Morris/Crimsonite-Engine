#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>



class Transform;
class Camera;
class CameraSettings;

//	Create a model matrix using a given Transform.
glm::mat4 CreateModelMatrix(const Transform & _transform);
//	Create a view matrix using given Camera reference.
glm::mat4 CreateViewMatrix(const Camera & _camera);
//	Create a projection matrix using given CameraSettings.
glm::mat4 CreateProjectionMatrix(const CameraSettings & _settings);
//	Create a projection matrix using given values.
glm::mat4 CreateProjectionMatrix(const float _fov, const float _ratio, const float _near, const float _far);