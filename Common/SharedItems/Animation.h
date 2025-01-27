#pragma once
#include <vector>
#include <string>
#include <glm.hpp>

//using namespace std;
class Shader;
class Model;
class Transform;
class Animation
{
public:
	Animation();

	void LoadAnimation(const std::string& path, std::vector<Model*>& vec);
	void Play(std::vector<Model*>& vec, Shader& shader, Transform& transform);
	//void LoadModelPosition(glm::vec3 position, std::vector<Model*>& vec);
private:
	int frame;
	bool frameLock;
	int frameOffset;
	
};

