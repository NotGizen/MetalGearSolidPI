#include "SceneGraph.h"
#include "Entity.h"
#include "Shader.h"
void SceneGraph::AddEntity(Entity* entity)
{
	entities.push_back(entity);
}

void SceneGraph::Update()
{
	for (Entity* i : entities)
	{
		i->UpdateSelfAndChild();
	}
}

void SceneGraph::Draw(Shader& shader)
{
	for (Entity* i : entities)
	{
		i->DrawSelfAndChildren(shader);
	}
}
