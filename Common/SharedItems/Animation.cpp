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
Animation::~Animation()
{
	for (Entity* i : modelFrames)
	{
		if(i != nullptr) delete i;
		
	}
}
void Animation::LoadAnimation(const std::string& pathInit, unsigned int maxFrames)
{
	for (int i = 1; i < maxFrames; i++)
	{
		std::stringstream newPath;
		newPath << pathInit;
		newPath << std::setfill('0') << std::setw(4) << i << ".obj";
		std::string path = newPath.str();
		Entity* entity = new Entity(path);
		
		modelFrames.push_back(entity);
	}
}

void Animation::Play(Entity& parent, unsigned int maxFrames)
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
	if (frame > maxFrames)
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

void Animation::Idle(Entity& parent)
{
	parent.RemoveUnusedChildren(modelFrames[frame]);
	frame = 0;
	frameOffset = 0;
	frameLock = true;
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


