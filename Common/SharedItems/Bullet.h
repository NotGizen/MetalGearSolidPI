#pragma once
#include "Entity.h"


class Model;
class Bullet : public Entity
    
{
public:
    Bullet(std::string const& path);
    ~Bullet();
    void Init();
 
    void FindTarget(glm::vec3 origin, glm::vec3 target);
    //btVector3 NormalizeTarget(btVector3 origin, btVector3 target);

    //GEt
    Model& GetBulletModel();
    float GetSpeed() const;
    

private:
    Model* bulletModel;
    glm::vec3 directionNormalized;
    float speed;
};

