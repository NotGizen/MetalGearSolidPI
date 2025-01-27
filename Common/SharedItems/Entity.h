#pragma once
#include <memory>
#include <list>
#include <vector>
#include "Transform.h"
#include <string>
#include <iostream>

class Model;
class Transform;
class Shader;


class Entity 
{
public:
	Entity();
	Entity(std::string const& path);
	~Entity();

	//Get
	Model& GetModel() const;
	
	void AddChild(Entity* child)
	{
		if (child != nullptr)
		{
			children.emplace_back(child);
			children.back()->parent = this;
		}
		
	}
	void RemoveAllChildren();
	void RemoveUnusedChildren(Entity* child);

	void SetParentToNull();

	void UpdateSelfAndChild();
	void DrawSelfAndChildren(Shader& shader);
	
	Transform* transform = nullptr;
	std::list<Entity*> children;
	
private:
	
	Entity* parent = nullptr;
	
	Model* model = nullptr;
};


