#include "DebugDrawer.h"
#include "Camera.h"
#include "SceneRenderer.h"
#include "Shader.h"

DebugDrawer::DebugDrawer() : myDebugMode(btIDebugDraw::DBG_DrawWireframe), greenColor(btVector3(0.f, 1.f, 0.f))
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

DebugDrawer::~DebugDrawer()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}



void DebugDrawer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	// draws a line between two contact points
	if (lifeTime > 0)
	{

	}
	btVector3 const startPoint = pointOnB;
	btVector3 const endPoint = pointOnB + normalOnB * distance;
	drawLine(startPoint, endPoint, color);
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	btVector3 newColor = color;
	//green COLORS
	// 
	//newColor = btVector3(0.f, 1.f, 0.f);
	// draws a simple line of pixels between points.
	lineVertices.push_back(from.x());
	lineVertices.push_back(from.y());
	lineVertices.push_back(from.z());
	lineVertices.push_back(newColor.x()); // Red component
	lineVertices.push_back(newColor.y()); // Green component
	lineVertices.push_back(newColor.z()); // Blue component

	// Add line end point
	lineVertices.push_back(to.x());
	lineVertices.push_back(to.y());
	lineVertices.push_back(to.z());
	lineVertices.push_back(newColor.x());
	lineVertices.push_back(newColor.y());
	lineVertices.push_back(newColor.z());

}



std::vector<float>& DebugDrawer::GetVertices()
{
	return lineVertices;
}
