#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
class Scene;
class Camera;
class Shader;
class Player;
class Model;
class Physics;
class DebugDrawer;
class Light;
class NoiseGenerator;
class Render
{
public:
	Render();
	~Render();

	void SetupDebugRender();
	void SetupNoiseRender();
	//Implement other components Draw Functions
	void RenderScene(Camera& camera, std::vector<Light*>& light, float dt, Scene& scene);
	void DrawPhysics(Scene& scene);
	Shader& GetDebugShader();
	Shader& GetSceneShader();
	Shader& GetTextureShader();
	unsigned int LoadTexture(const char* filename);

	//Noise
	unsigned int createPerlinNoiseTexture(const std::vector<float>& noiseData, int width, int height);
	void RenderPerlinNoise(unsigned int imageID, unsigned int noiseID, Camera& camera);
	void RenderTexture(unsigned int imageID, Camera& camera, Shader& shader, glm::vec3 position, float rotation = 0.f, glm::vec3 rotateAxis = glm::vec3(0.f, 0.f, 0.f), glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f));

private:
	void RenderDebugLines(DebugDrawer& debugDrawer, Camera& camera);
	//Implement VAOs & VBOs here
	unsigned int debugVAO, debugVBO;
	unsigned int noiseVAO, noiseVBO;
	Shader* debugShader;
	Shader* sceneShader;
	Shader* noiseShader;
	Shader* textureShader;
	
	
	
	
	
};

