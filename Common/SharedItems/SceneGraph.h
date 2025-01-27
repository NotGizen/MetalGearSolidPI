#pragma once
#include <vector>
class Entity;
class Shader;
class SceneGraph
{
public:
	void AddEntity(Entity* entity);
	void Update();
	void Draw(Shader& shader);

private:
	std::vector<Entity*> entities;
};

