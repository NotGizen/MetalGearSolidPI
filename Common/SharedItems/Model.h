#pragma once
#ifdef WINDOWS_BUILD
#include "glad/glad.h"
#endif
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Entity.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;


class Transform;
    unsigned int TextureFromFile(const char* path, const string& directory);
class Model : public Entity
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const& path ,bool gamma = false);
    ~Model();
    // draws the model, and thus all its meshes
    void Draw(Shader& shader, Transform& transform);
    //void SetPosition(glm::vec3 vec);
    //void SetScale(glm::vec3 vec);
    //void SetRotation(float angle);
    //glm::vec3 GetPosition() const;
    //glm::vec3& GetPositionRef();
    //glm::vec3& GetScaleRef();
    //float& GetRotationRef();
    std::vector<btVector3>& GetModelVertices() ;
   

private:
    //void UpdateTransform();
    std::vector<btVector3> modelVertices;
    /*glm::mat4 transform;
    glm::mat4 scale;
    glm::mat4 rotation;
    glm::mat4 translation;

    glm::vec3 scaleVec;
    glm::vec3 position;*/
    float angle;
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};




