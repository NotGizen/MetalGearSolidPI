#include "Camera.h"
#include "iostream"
#include "Common.h"
#include "SceneRenderer.h"
Camera::Camera(glm::vec3 position, glm::vec3 offset, glm::vec3 up): Front(glm::vec3(0.0f, 0.f, -1.0f)),
MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM), worldZ(glm::vec3(0.0f, 0.f, 1.0f)), Yaw(YAW), Pitch(PITCH), isTypeChanged(false)
{
	
	this->offset = offset;
	Position = position;
	WorldUp = up;
	camDir = IDLE;
	type = ThirdPerson; //Change camera Type
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
	//updateCameraVectors();
}

void Camera::Init(Scene& scene)
{
	
		SetCameraOffset(scene);
		updateCameraVectors();
	
}

void Camera::Update( glm::vec3 parentEPos , Scene& scene)
{
	switch (type)
	{
	case Camera::FirstPerson:
		Position = transform->position + parentEPos;
	updateCameraVectors();
		
		
		break;
	case Camera::ThirdPerson:
		Position = transform->position + parentEPos + glm::vec3(0.f, 5.f, 0.f);
		SetTargetPosition(parentEPos);
		//SetCameraPosition(targetPos);
		
		break;
	case Camera::Peek:
		//SetCameraPosition(targetPos);
	
		
		break;
	default:
		break;
	}
	SetCameraOffset(scene);
}

void Camera::GetLevel2Offset() const
{
	
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(45.f, ASPECT_RATIO, 0.1f, 300.f);
}

glm::vec3 Camera::GetOffset() const
{
	return offset;
}

glm::vec3 Camera::GetPosition() const
{
	return Position;
}

bool Camera::GetIsTypeChanged() const
{
	return isTypeChanged;
}

glm::vec3 Camera::GetOffsetFP() const
{
	return glm::vec3(0.f, 20.f ,0.f);
}

glm::vec3 Camera::LerpPosition(glm::vec3 a, glm::vec3 b, float t)
{
	glm::vec3 newPos = glm::mix(a, b, t);
	return newPos;
}

void Camera::SetCameraPosition(glm::vec3 playerPos)
{
	Position = playerPos + offset;
}

void Camera::SetTargetPosition(glm::vec3 targetPos)
{
	switch (type)
	{
	case Camera::FirstPerson:
		Front = glm::normalize(glm::vec3(0.f, 0.f, 1.f));
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
		break;
	case Camera::ThirdPerson:
		Front = glm::normalize(targetPos - Position);
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
		break;
	case Camera::Peek:
		Front = glm::normalize((targetPos + glm::vec3(0.f, 20.f, 0.f)) - Position);
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
		break;
	default:
		break;
	}
	
}

void Camera::SetNoTarget()
{

}

void Camera::SetCameraOffset(Scene& scene)
{
	switch (type)
	{
	case Camera::FirstPerson:
		//offset = glm::vec3(0.f, 10.f, 5.f);
		offset = glm::vec3(0.f, 20.f, 0.f);
		break;
	case Camera::ThirdPerson:
		if (scene.GetLevel() == Scene::LEVEL2)
		{
			offset = glm::vec3(0.f, 65.f, 40.f);
		}
		else
		{
		offset = glm::vec3(0.f, 65.f, -20.f);

		}
		break;
	case Camera::Peek:
		offset = glm::vec3(0.f, 20.f, -40.f);
		break;
	default:
		break;
	}
}

void Camera::SetIsTypeChanged(bool value)
{
	isTypeChanged = value;
}

void Camera::SetYaw(float value)
{
	Yaw = value;
}




