#include "AnimationManager.h"
#include "Model.h"
AnimationManager::AnimationManager()
{
	LoadAnimation("../Common/Assets/Standard_Walking/Standard_Walking", walkAnimation);

}

AnimationManager::~AnimationManager()
{
	for (Model* model : walkAnimation)
	{
		delete model;
		model = nullptr;
	}
}

 std::vector<Model*>& AnimationManager::GetWalkAnim()
{
    return walkAnimation;
}
