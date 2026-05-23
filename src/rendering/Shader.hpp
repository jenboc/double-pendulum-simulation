#pragma once

#include <glm/vec3.hpp>
#include <string>

namespace Engine::Rendering
{
    class Shader
    {
    public:
        virtual void Enable() = 0;
        virtual void Disable() = 0;
        virtual void SetFloat(const std::string& uniformName, float f) = 0;
        virtual void SetVector3(const std::string& uniformName, glm::vec3 vec) = 0;
    };
};
