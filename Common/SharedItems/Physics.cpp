#include "Physics.h"
#include "DebugDrawer.h"
#include "Actor.h"
#include "Model.h"
#include "Enemy.h"

Physics::Physics( Model& model)
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	collisionDispacher = new btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();
	constrainSolver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(collisionDispacher, broadphase, constrainSolver, collisionConfiguration);
	debugDrawer = new DebugDrawer();
	
	//CreateBox(plane, btVector3(10.f, 10.f, 10.f), btVector3(-10.f, 10.f, 0.f), 0.f, motionState2, rigidBody2);
	CreateTriangleShape(model);
	

}

Physics::~Physics()
{
	for (int i = world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		world->removeCollisionObject(obj);
		delete obj;
	}

	for (int j = 0; j < shapes.size(); j++)
	{
		btCollisionShape* shape = shapes[j];
		shapes[j] = 0;
		delete shape;
	}
	//delete collision shapes
	
	//delete plane;
	
	delete triangleMesh;
	triangleMesh = nullptr;

	
	delete world;
	world = nullptr;
	delete constrainSolver;
	delete broadphase;
	delete collisionDispacher;
	delete collisionConfiguration;
	delete debugDrawer;
}

void Physics::Init(Actor& actor)
{
	std::cout << "Physics Init" << std::endl;
	debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
	world->setDebugDrawer(debugDrawer);
	world->setGravity(btVector3(0.f, 0.f, 0.f));
	rigidBody1 = CreateBox(CollisionTypes::PLAYER, CollisionTypes::ENEMY, CollisionTypes::WALL, btVector3(3.f, 8.f, 3.f), GetActorSpawnPos(actor), 1.f);
	//CreateBox(btVector3(3.f, 3.f, 3.f), btVector3(0.f, 0.f, 0.f), testBox);
	rigidBody1->setSleepingThresholds(0, 0);
	rigidBody1->getWorldTransform().setOrigin(GetActorSpawnPos(actor));
	rigidBody1->setAngularFactor(btVector3(0, 0, 0));
	//testBox->setSleepingThresholds(0, 0);
	//testBox->getWorldTransform().setOrigin(btVector3(0.f, 0.f, 10.f));
	//testBox->setAngularFactor(btVector3(0, 0, 0));
	
	
	
}

void Physics::Update(float dt, Actor& actor)
{
	
	world->stepSimulation(dt, 10);
	btVector3 rayStart(0, 0, 0);  // Starting point of the ray
	btVector3 rayEnd(0, 40, 0);  // Ending point of the ray
	//PerformRaycast(rayStart, rayEnd, btVector3(1.f, 1.f, 0.f));

	
	btCollisionWorld::ClosestRayResultCallback rayCallback(rayStart, rayEnd);
	if (rigidBody1 != nullptr)
	{
		
		btVector3 newDirection = btVector3(actor.GetDirection().x, actor.GetDirection().y, actor.GetDirection().z);
		//std::cout << newDirection.z() << std::endl;
		btScalar newSpeed = btScalar(actor.GetSpeed());
		btVector3 velocity = newDirection * newSpeed;
		rigidBody1->setLinearVelocity(velocity);
		
	}
	//MoveUp(testbody);
}



btVector3 Physics::NormalizeVector(btVector3 origin, btVector3 target)
{
	btVector3 direction = target - origin;
	return direction.normalize();
}

void Physics::CreateTriangleShape(Model& model)
{
	vector<btVector3>& modelVector = model.GetModelVertices();
	triangleMesh = new btTriangleMesh();
	for (int i = 0; i < modelVector.size(); i += 3)
	{
		const btVector3& v0 = modelVector[i];
		const btVector3& v1 = modelVector[i + 1];
		const btVector3& v2 = modelVector[i + 2];
		triangleMesh->addTriangle(v0, v1, v2);

	}
	triangleShape = new btBvhTriangleMeshShape(triangleMesh, true);
	shapes.push_back(triangleShape);
	//shapes.push_back(triangleShape);
	btTransform transform;
	transform.setIdentity();

	btScalar mass(0.f);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		triangleShape->calculateLocalInertia(mass, localInertia);

	modelMotion = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, modelMotion, triangleShape, localInertia);
	modelBody = new btRigidBody(rbInfo);

	world->addRigidBody(modelBody, CollisionTypes::WALL, CollisionTypes::PLAYER | CollisionTypes:: ENEMY);

}

btRigidBody* Physics::CreateBox(CollisionTypes group, CollisionTypes mask, CollisionTypes mask2, btVector3 size, btVector3 origin, float massn)
{
	btBoxShape* obj = new btBoxShape(btVector3(size));
	shapes.push_back(obj);
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(origin);
	

	btScalar mass(massn);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		obj->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	motions.push_back(motionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, obj, localInertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfo);
	//bodies.push_back(rigidBody);
	
	world->addRigidBody(rigidBody, group, mask | mask2);
	return rigidBody;
}

btRigidBody* Physics::CreateDefaultBox(btVector3 size, btVector3 origin, float massn)
{
	btBoxShape* obj = new btBoxShape(btVector3(size));
	shapes.push_back(obj);
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(origin);


	btScalar mass(massn);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		obj->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	motions.push_back(motionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, obj, localInertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfo);
	//bodies.push_back(rigidBody);

	world->addRigidBody(rigidBody);
	return rigidBody;
}

btVector3 Physics::GetActorSpawnPos(Actor& actor) 
{
	btVector3 newOrigin = btVector3(actor.GetSpawnPosition().x, actor.GetSpawnPosition().y + 10, actor.GetSpawnPosition().z);
	return newOrigin;

}

glm::vec3 Physics::GetBodyPositon(btRigidBody& body)
{
	btVector3 btVec = body.getWorldTransform().getOrigin();
	glm::vec3 newVec = glm::vec3(btVec.x(), 0.f, btVec.z());
	return newVec;
}

btVector3 Physics::GetbtBodyPosition(btRigidBody& body)
{
	return body.getWorldTransform().getOrigin();
}

void Physics::DeleteRigidBody(btRigidBody*& body)
{
	world->removeRigidBody(body);
	btDefaultMotionState* motionState = dynamic_cast<btDefaultMotionState*>(body->getMotionState());
	delete motionState;
	motionState = nullptr;
	delete body;
	body = nullptr;

}

btDiscreteDynamicsWorld& Physics::GetWorld()
{
	return *world;
}

DebugDrawer& Physics::GetDebugDrawer()
{
	return *debugDrawer;
}

bool Physics::StopBodyOnCollision(btRigidBody& body)
{
	if (body.getNumConstraintRefs() > 0)
	{
		body.setLinearVelocity(btVector3(0, 0, 0));
		body.setAngularVelocity(btVector3(0, 0, 0));
		return true;
	}
	return false;
}

bool Physics::DetectCollision(btRigidBody* body1, btRigidBody* body2, void(Physics::*ActionFunction)(btRigidBody&, Actor&), Actor& actor)
{
	// After stepping the simulation, you can check for contacts.
   // This example assumes `playerRigidBody` and `testbox` exist.
	
	int numManifolds = world->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		// Check if the current manifold involves the specified rigid bodies
		if ((objA == body1 && objB == body2) || (objA == body2 && objB == body1)) {
			int numContacts = contactManifold->getNumContacts();

			// Check if there's at least one contact point
			for (int j = 0; j < numContacts; j++) {
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance() <= 0.f) {  // Objects are in contact
					//std::cout << "Collision detected!" << std::endl;
					//Accept a function to trigger an action
						(this->*ActionFunction)(*body2, actor);		
					return true;
				}
			}
		}
	}
	return false;
}

bool Physics::DetectDefaultCollision(btRigidBody* body1, btRigidBody* body2)
{
	int numManifolds = world->getDispatcher()->getNumManifolds();

	for (int i = 0; i < numManifolds; i++) {
		btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
		const btCollisionObject* objA = contactManifold->getBody0();
		const btCollisionObject* objB = contactManifold->getBody1();

		// Check if the current manifold involves the specified rigid bodies
		if ((objA == body1 && objB == body2) || (objA == body2 && objB == body1)) {
			int numContacts = contactManifold->getNumContacts();

			// Check if there's at least one contact point
			for (int j = 0; j < numContacts; j++) {
				btManifoldPoint& pt = contactManifold->getContactPoint(j);
				if (pt.getDistance() <= 0.f) {  // Objects are in contact
					//std::cout << "Collision detected!" << std::endl;
					//Accept a function to trigger an action
					
					return true;
				}
			}
		}
	}
	return false;
}

btRigidBody* Physics::GetPlayerRigidBody() 
{
	return rigidBody1;
}

btRigidBody*& Physics::GetPlayerBodyRef()
{
	return rigidBody1;
}

bool Physics::PerformRaycast(btVector3 start, btVector3 end, btVector3 color, CollisionTypes objectColliding)
{
	btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
	world->rayTest(start, end, rayCallback);
	if (rayCallback.hasHit())
	{

		btRigidBody* hitBody = const_cast<btRigidBody*>(btRigidBody::upcast(rayCallback.m_collisionObject));
		if (hitBody && (hitBody->getBroadphaseHandle()->m_collisionFilterGroup & objectColliding))
		{
		
			
			debugDrawer->drawLine(start, end, color);
			return true;
		}
		
	}
	
	return false;
}

bool Physics::PerformPatrollingRaycast(btVector3 start, btVector3 end, btVector3 color, CollisionTypes objectColliding)
{
	btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
	world->rayTest(start, end, rayCallback);
	if (rayCallback.hasHit())
	{
		btRigidBody* hitBody = const_cast<btRigidBody*>(btRigidBody::upcast(rayCallback.m_collisionObject));
		if (hitBody && (hitBody->getBroadphaseHandle()->m_collisionFilterGroup == objectColliding))
		{	
			return true;
		}
	}
	
	debugDrawer->drawLine(start, end, color);
	return false;
}

bool Physics::PerformNoCollisionRaycast(btVector3 start, btVector3 end, btVector3 color)
{
	btCollisionWorld::ClosestRayResultCallback rayCallback(start, end);
	world->rayTest(start, end, rayCallback);
	

		



			debugDrawer->drawLine(start, end, color);
			return true;
		

	
	
}

void Physics::MoveUp(btRigidBody& body, Actor& actor )
{
	
	
	btVector3 posDir = btVector3(0.f, 0.f, 1.f);
	btScalar velocity = actor.GetSpeed();
	btVector3 newPos = posDir * velocity;
	body.setLinearVelocity(newPos);

	
	
	actor.posDir.front = true;
	actor.posDir.back = false;
	actor.posDir.left = false;
	actor.posDir.right = false;
	//std::cout <<  body.getWorldTransform().getOrigin().z() << std::endl;
}

void Physics::MoveDown(btRigidBody& body, Actor& actor)
{
	btVector3 posDir = btVector3(0.f, 0.f, -1.f);
	btScalar velocity = actor.GetSpeed();
	btVector3 newPos = posDir * velocity;
	body.setLinearVelocity(newPos);
	actor.posDir.front = false;
	actor.posDir.back = true;
	actor.posDir.left = false;
	actor.posDir.right = false;
}

void Physics::MoveLeft(btRigidBody& body, Actor& actor)
{
	btVector3 posDir = btVector3(1.f, 0.f, 0.f);
	btScalar velocity = actor.GetSpeed();
	btVector3 newPos = posDir * velocity;
	body.setLinearVelocity(newPos);
	actor.posDir.front = false;
	actor.posDir.back = false;
	actor.posDir.left = true;
	actor.posDir.right = false;
}

void Physics::MoveRight(btRigidBody& body, Actor& actor)
{
	btVector3 posDir = btVector3(-1.f, 0.f, 0.f);
	btScalar velocity = actor.GetSpeed();
	btVector3 newPos = posDir * velocity;
	body.setLinearVelocity(newPos);
	actor.posDir.front = false;
	actor.posDir.back = false;
	actor.posDir.left = false;
	actor.posDir.right = true;
}

void Physics::MoveNone(btRigidBody& body)
{
	
		body.setLinearVelocity(btVector3(0.f, 0.f, 0.f));
	
}

void Physics::MoveContinue(btRigidBody& body, Actor& actor)
{
	
	btScalar velocity = actor.GetSpeed();
	if (actor.posDir.front == true)
	{
		btVector3 posDir = btVector3(0.f, 0.f, 1.f);
		btVector3 newPos = posDir * velocity;
		body.setLinearVelocity(newPos);
	}
	if (actor.posDir.back == true)
	{
		btVector3 posDir = btVector3(0.f, 0.f, -1.f);
		btVector3 newPos = posDir * velocity;
		body.setLinearVelocity(newPos);
	}
	if (actor.posDir.left == true)
	{
		btVector3 posDir = btVector3(1.f, 0.f, 0.f);
		btVector3 newPos = posDir * velocity;
		body.setLinearVelocity(newPos);
	}
	if (actor.posDir.right == true)
	{
		btVector3 posDir = btVector3(-1.f, 0.f, 0.f);
		btVector3 newPos = posDir * velocity;
		body.setLinearVelocity(newPos);
	}
}

btRigidBody* Physics::GetRoomBody() 
{
	return modelBody;
}




