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
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

    class Shader
    {
    public:
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath);

        void use();
        //void Draw();
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec3(const std::string& name, glm::vec3 value) const;
        void setMat4(const std::string& name, const glm::mat4 mat) const;

    private:
        void checkCompileErrors(unsigned int shader, std::string type);
      
    };


