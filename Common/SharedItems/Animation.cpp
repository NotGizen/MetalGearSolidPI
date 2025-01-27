#include "Animation.h"
#include "Model.h"
#include <sstream>
#include <iomanip>
#include "Shader.h"
#include "Transform.h"
Animation::Animation()
{
	frame = 0;
	frameLock = true;
}
void Animation::LoadAnimation(const std::string& pathInit, std::vector<Model*>& vec)
{
	for (int i = 1; i < 29; i++)
	{
		std::stringstream newPath;
		newPath << pathInit;
		newPath << std::setfill('0') << std::setw(4) << i << ".obj";
		std::string path = newPath.str();
		Model* model = new Model(path);
		vec.push_back(model);
	}
}

void Animation::Play(std::vector<Model*>& vec, Shader& shader, Transform& transform)
{
	if (frame > 27)
	{
		frame = 0;
	}
	
		//LoadModelPosition(position, vec);
		vec[frame]->Draw(shader, transform);
		 

	if (!frameLock)
	{

		++frame;
		frameLock = true;
	}
	if (frameOffset >= 1)
	{
		frameLock = false;
		frameOffset = 0;
	}
	else
	{
		++frameOffset;
	}
}

//void Animation::LoadModelPosition(glm::vec3 position, std::vector<Model*>& vec)
//{
//	for (Model* model : vec)
//	{
//		model->SetPosition(position);
//	}
//}
