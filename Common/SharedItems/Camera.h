#pragma once
#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#include <GLES2/gl2.h>
#endif
#ifdef WINDOWS_BUILD
#include "glad/glad.h"
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods

// Default camera values


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Scene;
class IGraphics;
class Camera : public Entity
{
public:
    enum Camera_Type
    {
        FirstPerson,
        ThirdPerson,
        Peek
    }type;
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        IDLE
    }camDir;
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
 

    // euler Angles

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3 offset = glm::vec3(0.f, 60.f, -40.f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
 
    void Init(Scene& scene);
    void Update( glm::vec3 parentEPos, Scene& scene);
    void GetLevel2Offset() const;
    
  
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        // Adjust the offset by sensitivity
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        // Update yaw and pitch
        Yaw += xoffset;
        Pitch += yoffset;

        // Constrain pitch to avoid flipping
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Wrap yaw to prevent overflow
        if (Yaw > 360.0f)
            Yaw -= 360.0f;
        if (Yaw < 0.0f)
            Yaw += 360.0f;

        // update Front, Right and Up Vectors using the updated Euler angles
       // updateCameraVectors();
    }
    //GET
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::vec3 GetOffset() const;
    glm::vec3 GetPosition() const;
    bool GetIsTypeChanged() const;
    glm::vec3 GetOffsetFP() const;
    glm::vec3 LerpPosition(glm::vec3 a, glm::vec3 b, float t);

    //Set
    void SetCameraPosition(glm::vec3 playerPos);
    void SetTargetPosition(glm::vec3 targetPos);
    void SetNoTarget();
    void SetCameraOffset(Scene& scene);
    void SetIsTypeChanged(bool value);
    void SetYaw(float value);
   

private:
    const float YAW = 0.0f;
    const float PITCH = 0.0f;
    const float SPEED = 50.f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    glm::vec3 offset;
    glm::vec3 worldZ;
    bool isTypeChanged;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {

        
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw - 90.f)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw - 90.f)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        transform->rotation.y = -Yaw;
        transform->rotation.x = Pitch;
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

