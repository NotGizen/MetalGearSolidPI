#pragma once
#include <vector>
#include "Animation.h"
class Model;

class AnimationManager : public Animation
{
public:
	AnimationManager();
	~AnimationManager();
	 std::vector<Model*>& GetWalkAnim();
private:
	std::vector<Model*> walkAnimation;
};

