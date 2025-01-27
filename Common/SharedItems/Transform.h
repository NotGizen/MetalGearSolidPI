#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Transform
{
public:
	Transform();
	glm::mat4 getLocalModelMatrix();
	glm::mat4& GetGlobalModelMatrix();
	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
private:

};

