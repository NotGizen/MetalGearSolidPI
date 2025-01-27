#include "Light.h"
#include <iostream>
#include <algorithm>
Light::Light() : timer(0.f), isLightOn(true), flickeringCount(0), defaultValue(0.5f) //default 0.6f
{
	position = glm::vec3(0.f, 0.f, 0.f);
	ambientValue = glm::vec3(0.2f, 0.2f, 0.2f);
	diffuseValue = glm::vec3(1.f, 1.f, 1.f);
	specularValue = glm::vec3(0.4f, 0.4f, 0.4f);
	direction = glm::vec3(0.f, 1.f, 0.f);
	lightType = Spinning; // Change lightType
	spinningTimer = 0.f;
}

void Light::Update(float dt)
{
	switch (lightType)
	{
	case Flickering:
		FlickerLight(dt);
		break;
	case Spinning:
		SpinLight(dt);
		break;
	default:
		break;
	}
	
}

void Light::FlickerLight(float dt) 
{
	timer += dt;
	if (flickeringCount <= 3)
	{
		if (isLightOn)
		{
			if (timer > 0.1f)
			{
				isLightOn = false;
				diffuseValue = glm::vec3(0.f, 0.f, 0.f);
				specularValue = glm::vec3(0.f, 0.f, 0.f);
				timer = 0.f;
				flickeringCount++;
			}

		}
		else
		{
			if (timer > 0.1f)
			{
				isLightOn = true;
				diffuseValue = glm::vec3(1.f, 1.f, 1.f);
				specularValue = glm::vec3(0.4f, 0.4f, 0.4f);
				timer = 0.f;
			}
		}
	}
	else
	{
		if (timer > 0.7f)
		{
			flickeringCount = 0;
		}
	}

}

void Light::SpinLight(float dt)
{
	spinningTimer += dt * 50; // spinning speed
	
	direction.x = cos(glm::radians(spinningTimer));
	direction.z = sin(glm::radians(spinningTimer));
	
	ambientValue.x = std::max(static_cast<float>(sin(glm::radians(spinningTimer) * 3.f)), defaultValue);
	ambientValue.y = defaultValue;
	ambientValue.z = defaultValue;
	
}

void Light::SetPosition(glm::vec3 newposition)
{
	this->position = newposition;
}

glm::vec3& Light::GetPosition() 
{
	return position;
}

glm::vec3& Light::GetAmbient() 
{
	return ambientValue;
}

glm::vec3& Light::GetDiffuse() 
{
	return diffuseValue;
}

glm::vec3& Light::GetSpecular() 
{
	return specularValue;
}

glm::vec3& Light::GetDirection() 
{
	direction = glm::normalize(direction);
	return direction;
}

float& Light::GetCutOff() 
{
	return cutoff;
}
