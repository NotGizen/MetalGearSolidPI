#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
class DebugDrawer;
class Shader;
class Actor;
class Model;
class Enemy;
enum CollisionTypes
{
	PLAYER = 1,
	WALL = 2 ,
	ENEMY = 3
};
class Physics
{
public:
	Physics( Model& model);
	~Physics();
	void Init(Actor& actor);
	void Update(float dt, Actor& actor);
	
	//TESTING
	btVector3 NormalizeVector(btVector3 origin, btVector3 target);
	void CreateTriangleShape(Model& model);
	btRigidBody* CreateBox( CollisionTypes group, CollisionTypes mask, CollisionTypes mask2, btVector3 size, btVector3 origin = btVector3(0.f, 0.f, 0.f),float mass = 0.f);
	btRigidBody* CreateDefaultBox(btVector3 size, btVector3 origin = btVector3(0.f, 0.f, 0.f), float massn = 0.f);
	btVector3 GetActorSpawnPos(Actor& actor);
	glm::vec3 GetBodyPositon(btRigidBody& body);
	btVector3 GetbtBodyPosition(btRigidBody& body);
	void DeleteRigidBody(btRigidBody*& body);
	btDiscreteDynamicsWorld& GetWorld();
	DebugDrawer& GetDebugDrawer();
	bool StopBodyOnCollision(btRigidBody& body);
	bool DetectCollision(btRigidBody* body1, btRigidBody* body2, void(Physics::*ActionFunction)(btRigidBody&, Actor&), Actor& actor);
	bool DetectDefaultCollision(btRigidBody* body1, btRigidBody* body2);
	btRigidBody* GetPlayerRigidBody();
	btRigidBody*& GetPlayerBodyRef();
	bool PerformRaycast(btVector3 start, btVector3 end, btVector3 color, CollisionTypes objectColliding);
	bool PerformPatrollingRaycast(btVector3 start, btVector3 end, btVector3 color, CollisionTypes objectColliding);
	bool PerformNoCollisionRaycast(btVector3 start, btVector3 end, btVector3 color);

	//MovingRigidBodies Functions
	void MoveUp(btRigidBody& body, Actor& actor);
	void MoveDown(btRigidBody& body, Actor& actor);
	void MoveLeft(btRigidBody& body, Actor& actor);
	void MoveRight(btRigidBody& body, Actor& actor);
	void MoveNone(btRigidBody& body);
	void MoveContinue(btRigidBody& body, Actor& actor);

	//Get
	btRigidBody* GetRoomBody();



private:
	btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
	btCollisionDispatcher* collisionDispacher = nullptr;
	btDbvtBroadphase* broadphase = nullptr;
	btSequentialImpulseConstraintSolver* constrainSolver = nullptr;
	btDiscreteDynamicsWorld* world = nullptr;

	DebugDrawer* debugDrawer = nullptr;

	btBvhTriangleMeshShape* triangleShape = nullptr;
	btTriangleMesh* triangleMesh = nullptr;
	btBoxShape* boxShape = nullptr;
	btBoxShape* plane = nullptr;
	btDefaultMotionState* motionState1 = nullptr;
	btDefaultMotionState* motionState2 = nullptr;
	btDefaultMotionState* modelMotion = nullptr;
	btRigidBody* modelBody = nullptr;
	btRigidBody* rigidBody1 = nullptr;
	btRigidBody* rigidBody2 = nullptr;
	std::vector<btVector3>modelVertices;
	std::vector<btCollisionShape*>shapes;
	std::vector<btMotionState*>motions;
	std::vector<btRigidBody*>bodies;
	std::vector<btTriangleMesh*>triangleMeshes;

	CollisionTypes collisionTypes;
	//btRigidBody* testBox = nullptr;
	//TEST RAYCAST
	
};

