#pragma once
#include <vector>
#include <string>
#include <glm.hpp>


//using namespace std;
class Shader;
class Model;
class Transform;
class Entity;
class Animation 
{
public:
	Animation();

	void LoadAnimation(const std::string& path);
	void Play(Entity& parent);
	//void LoadModelPosition(glm::vec3 position, std::vector<Model*>& vec);
	std::vector<Entity*>& GetModelFrame();
	unsigned int GetFrame() const; 

private:
	unsigned int frame;
	bool frameLock;
	int frameOffset;
	std::vector<Entity*>modelFrames;
	
};

