#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum LightType
{
	Flickering,
	Spinning,
	SpotLight
};
class Light
{
public:
	Light();
	void Update(float dt);
	void FlickerLight(float dt) ;
	void SpinLight(float dt);
	//Set
	void SetPosition(glm::vec3 position);

	//Get
	glm::vec3& GetPosition() ;
	glm::vec3& GetAmbient() ;
	glm::vec3& GetDiffuse() ;
	glm::vec3& GetSpecular() ;
	glm::vec3& GetDirection() ;
	float& GetCutOff() ;
	LightType lightType;

private:
	int flickeringCount;
	float timer;
	float spinningTimer;
	bool isLightOn;
	glm::vec3 position;
	glm::vec3 diffuseValue;
	glm::vec3 specularValue;
	glm::vec3 ambientValue;
	glm::vec3 direction;
	float cutoff;
	float defaultValue;
};

