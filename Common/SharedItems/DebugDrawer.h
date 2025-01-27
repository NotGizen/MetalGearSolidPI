#pragma once

#ifdef WINDOWS_BUILD
#include <GLFW/glfw3.h>
#else
#include <GLES2/gl2.h>
#endif

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#include <GLES2/gl2.h>
#endif

#ifdef WINDOWS_BUILD
#include "glad/glad.h"
#endif

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "LinearMath/btIDebugDraw.h"

class Shader;

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer();
	~DebugDrawer();
	virtual void setDebugMode(int debugMode) override { myDebugMode = debugMode; }
	virtual int getDebugMode() const override { return myDebugMode; }

	virtual void  drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
	virtual void  drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	
	// unused
	virtual void  reportErrorWarning(const char* warningString) override
	{
		if (warningString != nullptr)
		{
		}
			return;

	}
	virtual void  draw3dText(const btVector3& location, const char* textString) override
	{
		if (textString != nullptr)
		{
			 location;
		}
			return;

	}
	std::vector<float>& GetVertices();
private:
	int myDebugMode;
	unsigned int VAO, VBO;
	std::vector<float> lineVertices;
	btVector3 greenColor;
};

