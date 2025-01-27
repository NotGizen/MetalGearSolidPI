#pragma once
#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#include <GLES2/gl2.h>
#endif
#ifdef WINDOWS_BUILD
#include "glad/glad.h"
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "Render.h"
#include "Entity.h"
class Model;
class Shader;
class AnimationManager;
class Physics;
//class Transform;


class Actor : public Entity ,public Render
{
public:

	struct MoveDirection
	{
		bool front;
		bool back;
		bool left;
		bool right;
		bool idle;
	}posDir;
	MoveDirection savedDir;
	/*enum DirectionPosition
	{
		UP, 
		DOWN,
		LEFT,
		RIGHT,
		IDLE
	}dirPos;*/
	Actor(const std::string& path);
	virtual ~Actor();
	
	 void Update();
	//virtual void Draw(Shader& shader);
	//void MovingDirection(AnimationManager& animator);
	

	//Set
	
	 void SetSpeed(float speed);
	 void SetIsAlive(bool isAlive);
	 void SetLastRotationDir(float value);
	//Get 
	 void SetLerpTimer(float value) ;
	 float GetLerpTimer();
	 float CalculateRotation();
	 float GetLastRotation();
	glm::vec3 GetDirection() const;
	glm::vec3 GetSpawnPosition() const;
	float GetSpeed() const;
	const int& GetHealth();
	bool GetIsImmortal() const;
	bool GetIsAlive() const;
	void ReceiveDamage();
	

protected:
	//Model* model = nullptr;
	//AnimationManager* animator = nullptr;
	float speed;
	float lastAngle;
	glm::vec3 position;
	glm::vec3 scale;
	float angle;
	glm::vec3 ZAxis;
	glm::vec3 XAxis;
	glm::vec3 direction;

	glm::vec3 spawnPosition;

	int health;
	bool isAlive;
	glm::vec3 rotationDir;
	float lastRotationDir;
	bool changedDirection;
	float rotationLerpTimer;
	bool isImmortal;

	
};

