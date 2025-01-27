#include "Animation.h"
#include "Model.h"
#include <sstream>
#include <iomanip>
#include "Shader.h"
#include "Transform.h"
#include "Entity.h"
Animation::Animation()
{
	frame = 0;
	frameOffset = 0;
	frameLock = true;
}
void Animation::LoadAnimation(const std::string& pathInit)
{
	for (int i = 1; i < 29; i++)
	{
		std::stringstream newPath;
		newPath << pathInit;
		newPath << std::setfill('0') << std::setw(4) << i << ".obj";
		std::string path = newPath.str();
		Entity* entity = new Entity(path);
		
		modelFrames.push_back(entity);
	}
}

void Animation::Play(Entity& parent)
{
	parent.RemoveUnusedChildren(modelFrames[frame]);
	if (!frameLock)
	{
		frameLock = true;
	}
	if (frameOffset >= 1)
	{
		frameLock = false;
		frameOffset = 0;
	++frame;
	if (frame > 27)
	{
		frame = 0;
	}
	}
	else
	{
		++frameOffset;
	}
	parent.AddChild(modelFrames[frame]);
		
}

std::vector<Entity*>& Animation::GetModelFrame()
{
	return modelFrames;
}

unsigned int Animation::GetFrame() const
{
	return frame;
}

//void Animation::LoadModelPosition(glm::vec3 position, std::vector<Model*>& vec)
//{
//	for (Model* model : vec)
//	{
//		model->SetPosition(position);
//	}
//}
