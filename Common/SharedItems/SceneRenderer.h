#pragma once
#include "Camera.h"
#include "Model.h"
#include "Shader.h"

class Entity;
class Light;
class Camera;
class Model;
class Shader;
class Player;
class Animation;
class Gun;
class Bullet;
class Actor;
class Enemy;
class Physics;
class Render;
class Scene
{
public:
	enum Level
	{
		LEVEL1,
		LEVEL2
	}level;
	struct EnemyStruct
	{
		btRigidBody* body;
		Enemy* entity;
		float timer;
		bool isAlert;
		bool bulletHit;
	};
	struct BulletStruct
	{
		btRigidBody* body;
		Entity* entity;
		bool isPlayer;
	};
	Scene();
	~Scene();
	void Init(Render& render);
	//Level2
	void SetupLevel2();
	void SetUpEntities();
	void CameraChangeSetup();
	void UpdateEntities(float dt, glm::vec3 colliderPosition);
	void Update(float dt);
	void UpdateBullet(float dt);
	void Draw(Shader& shader, Render& render, float dt);
	void CameraLerping(float dt);
	void ActorDirectionLerping(float dt, Actor& actor);
	void CreateBullet(bool isEnemyShooting, Entity* enemy, btRigidBody*);
	
	void DestroyEnemy();
	//Physics
	//void SetupPhysicsObjects(btRigidBody& body, bool isPath, btVector3 size, btVector3 position, float mass = 0.f);
	void InitPhysicsCollision(btRigidBody& body);
	void CreatePathBox(btVector3 position);
	void CreateEnemy( btVector3 position);
	void ShootPlayerGun();
	//Set
	void SetIsChangingRoom(bool value);
	void SetDidUserWin(bool value);
	//Get
	bool GetDidUserWin() const;
	Level GetLevel() const;
	bool GetIsChangingRoom() const;
	Physics& GetPhysics() const;
	Entity& GetRoom() const;
	Camera& GetCamera() const;
	Camera* GetCameraP();
	Player& GetPlayer() const;
	Entity& GetPlayerCollider() const;
	Gun& GetGun() const;
	std::vector<Light*>& GetLights();

	//Clean
	void DeleteBullet();

	

	
private:
	void UpdateScenePhysics(float dt);
	//AnimationManager* animator;
	 
	//Entities
	Physics* physics = nullptr;
	Player* player = nullptr;
	Entity* room = nullptr;
	Camera* camera = nullptr;
	//Enemy* enemy = nullptr;
	Gun* gun = nullptr;
	Entity* sceneRoot = nullptr;
	Entity* playerCollider = nullptr;
	//std::vector<Entity*> bullets;
	std::vector<BulletStruct> bullets;
	std::vector<EnemyStruct> enemies;
	std::vector<Light*> lights;
	Animation* playerAnimator;
	//std::vector<Entity*> playerAnimation;
	std::vector<btRigidBody*> pathBoxes;
	btRigidBody* pathBoxA = nullptr;
	btRigidBody* pathBoxB = nullptr;

	unsigned int door;
	btRigidBody* doorBody = nullptr;
	unsigned int exitDoor;
	btRigidBody* exitDoorBody = nullptr;
	unsigned int shotTexture;
	
	//Vars
	int nrLights;
	int nrPathPoints;
	bool isCameraLerping;
	float lerpTimer;
	float rotationLerpTimer;
	bool canShoot;
	float changingLevelTimer;
	bool isPlayerBoxDead;
	bool isChangingRoom;
	bool isLevel2Setup;
	bool isExitDoorLoaded;
	bool didUserWin;
	int changingRoomCounter;
	//bool bulletHit;
	float raycastTimer;
	btVector3 previousPlayerPos;
	btVector3 previousEnemyPos;
	float playerGunTimer;
	bool isFire;
	int playerAnimFrame;
	
};

