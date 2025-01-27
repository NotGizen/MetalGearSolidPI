#include "Entity.h"
#include "Transform.h"
#include "Model.h"
#include "Shader.h"



Entity::Entity()
{
    
    transform = new Transform();
}

Entity::Entity(std::string const& path)
{
    
    model = new Model(path);
    transform = new Transform();
}
Entity::~Entity()
{
    delete model;
    model = nullptr;
    delete transform;
    transform = nullptr;

    children.clear();
   
}

Model& Entity::GetModel() const
{
    return *model;
}

void Entity::RemoveAllChildren()
{
    for (Entity* child : children)
    {
        child->parent = nullptr;
    }
    children.clear();
}

void Entity::RemoveUnusedChildren(Entity* child)
{
        std::list<Entity*>::iterator it = std::find(children.begin(), children.end(), child);
        if (it != children.end())
        {
            children.erase(it);
        }
        else
        {
            std::cout << "Child not Found" << std::endl;
        }
}

void Entity::SetParentToNull()
{
    parent = nullptr;
}

void Entity::UpdateSelfAndChild()
{
        if (parent)
            transform->modelMatrix = parent->transform->modelMatrix * transform->getLocalModelMatrix();
        else
            transform->modelMatrix = transform->getLocalModelMatrix();

        for (auto&& child : children)
        {
            child->UpdateSelfAndChild();
        }  
}

void Entity::DrawSelfAndChildren(Shader& shader)
{
    if (model)
    {
        model->Draw(shader, *transform);
    }
    for (const auto& child : children) {

        child->DrawSelfAndChildren(shader);

    }
}


