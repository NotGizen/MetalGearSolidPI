#include "Render.h"
#include "Shader.h"
#include "SceneRenderer.h"
#include "Player.h"
#include "Camera.h"
#include "Model.h"
#include "Physics.h"
#include "DebugDrawer.h"
#include "Light.h"

#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Render::Render() 
{
	debugShader = new Shader("../Common/Shaders/debug.vs", "../Common/Shaders/debug.fs");
	sceneShader = new Shader("../Common/Shaders/model_loading.vs", "../Common/Shaders/model_loading.fs");
	noiseShader = new Shader("../Common/Shaders/noise.vs", "../Common/Shaders/noise.fs");
	textureShader = new Shader("../Common/Shaders/textureDefault.vs", "../Common/Shaders/textureDefault.fs");
	SetupDebugRender();
	SetupNoiseRender();
	
}

Render::~Render()
{
	glDeleteBuffers(1, &debugVBO);
	glDeleteVertexArrays(1, &debugVAO);

	delete debugShader;
	delete sceneShader;
	delete noiseShader;
	delete textureShader;
}

void Render::SetupDebugRender()
{
	glGenVertexArrays(1, &debugVAO);
	glGenBuffers(1, &debugVBO);

	glBindVertexArray(debugVAO);
	glBindBuffer(GL_ARRAY_BUFFER, debugVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Render::SetupNoiseRender()
{
	float noiseVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	};
	glGenVertexArrays(1, &noiseVAO);
	glGenBuffers(1, &noiseVBO);

	glBindVertexArray(noiseVAO);
	glBindBuffer(GL_ARRAY_BUFFER, noiseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(noiseVertices), noiseVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Render::RenderScene(Camera& camera, std::vector<Light*>& light, float dt, Scene& scene)
{
	sceneShader->use();
	
	
	//Light Properties
		sceneShader->setVec3("light[0].position", light[0]->GetPosition());
		sceneShader->setVec3("light[0].direction", light[0]->GetDirection());
		sceneShader->setVec3("light[0].ambient", light[0]->GetAmbient());
		sceneShader->setVec3("light[0].diffuse", light[0]->GetDiffuse());
		sceneShader->setVec3("light[0].specular", light[0]->GetSpecular());
		sceneShader->setFloat("light[0].constant", 1.0f);
		sceneShader->setFloat("light[0].linear", 0.022f);
		sceneShader->setFloat("light[0].quadratic", 0.0019f);
		sceneShader->setFloat("light[0].cutOff", glm::cos(glm::radians(30.5f)));

		sceneShader->setVec3("light[1].position", light[1]->GetPosition());
		sceneShader->setVec3("light[1].direction", light[1]->GetDirection());
		sceneShader->setVec3("light[1].ambient", light[1]->GetAmbient());
		sceneShader->setVec3("light[1].diffuse", light[1]->GetDiffuse());
		sceneShader->setVec3("light[1].specular", light[1]->GetSpecular());
		sceneShader->setFloat("light[1].constant", 1.0f);
		sceneShader->setFloat("light[1].linear", 0.007f);
		sceneShader->setFloat("light[1].quadratic", 0.0002f);
		sceneShader->setFloat("light[1].cutOff", glm::cos(glm::radians(30.5f)));
	
	sceneShader->setVec3("viewPos", camera.Position);
	glm::mat4 projection = camera.GetProjectionMatrix();
	glm::mat4 view = camera.GetViewMatrix();
	sceneShader->setMat4("projection", projection);
	sceneShader->setMat4("view", view);

	scene.Draw(*sceneShader, *this, dt);
	
}

void Render::DrawPhysics(Scene& scene)
{
	scene.GetPhysics().GetWorld().debugDrawWorld();
	RenderDebugLines(scene.GetPhysics().GetDebugDrawer(), scene.GetCamera());
}





Shader& Render::GetDebugShader()
{
	return *debugShader;
}

Shader& Render::GetSceneShader()
{
	return *sceneShader;
}

Shader& Render::GetTextureShader()
{
	return *textureShader;
}

unsigned int Render::LoadTexture(const char* filename)
{
	int width, height, channels;
	unsigned char* data = stbi_load(filename, &width, &height, &channels, 4);
	if (data == nullptr)
	{
		std::cerr << "Failed to load texture: " << filename << std::endl;
		return 0;
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	return textureID;
}

unsigned int Render::createPerlinNoiseTexture(const std::vector<float>& noiseData, int width, int height)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);	

	// Upload the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, noiseData.data());

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

void Render::RenderPerlinNoise(unsigned int imageID, unsigned int noiseID, Camera& camera)
{
	
	
	// Use the shader program
	noiseShader->use();

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, imageID);
	noiseShader->setInt("texture1", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, noiseID);
	noiseShader->setInt("texture2", 1);

	// Bind the VAO
	glBindVertexArray(noiseVAO);

	noiseShader->setMat4("projection", camera.GetProjectionMatrix());
	noiseShader->setMat4("view", camera.GetViewMatrix());
	glm::mat4 model(1.f);
	model = glm::translate(model, glm::vec3(0.f, -10, 0.f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
	model = glm::scale(model, glm::vec3(400.f, 400.f, 0.f));
	noiseShader->setMat4("model", model);

	// Draw the quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Unbind
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Render::RenderTexture(unsigned int imageID, Camera& camera, Shader& shader, glm::vec3 position, float rotation,glm::vec3 rotateAxis, glm::vec3 scale)
{
	// Use the shader program
	shader.use();

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, imageID);
	shader.setInt("texture1", 0);

	

	// Bind the VAO
	glBindVertexArray(noiseVAO);

	shader.setMat4("projection", camera.GetProjectionMatrix());
	shader.setMat4("view", camera.GetViewMatrix());
	glm::mat4 model(1.f);
	//model = glm::translate(model, glm::vec3(0.f, 10.f, 120.f));
	//model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 0.f, 1.f));
	//model = glm::scale(model, glm::vec3(20.f, 15.f, 0.f));
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(rotation), rotateAxis);
	model = glm::scale(model, scale);
	shader.setMat4("model", model);

	// Draw the quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Unbind
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}






void Render::RenderDebugLines(DebugDrawer& debugDrawer, Camera& camera)
{
	// Bind the VAO
	glBindVertexArray(debugVAO);

	// Bind and upload the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, debugVBO);
	glBufferData(GL_ARRAY_BUFFER, debugDrawer.GetVertices().size() * sizeof(float), debugDrawer.GetVertices().data(), GL_DYNAMIC_DRAW);

	//bind uniforms
	debugShader->use();
	debugShader->setMat4("projection", camera.GetProjectionMatrix());
	debugShader->setMat4("view", camera.GetViewMatrix());
	glm::mat4 model(1.f);
	debugShader->setMat4("model", model);


	glDrawArrays(GL_LINES, 0, static_cast<int>(debugDrawer.GetVertices().size()) / 6); //Check erros with PI here

	debugDrawer.GetVertices().clear();

	// Unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
