#pragma once

#include <glm/glm.hpp>
#include <string>

namespace Engine::Rendering
{
    constexpr int GLSL_VERSION = 330;

    class Shader
    {
    public:
        virtual void Enable() = 0;
        virtual void Disable() = 0;
        virtual void SetFloat(const std::string& uniformName, float f) = 0;
        virtual void SetVector3(const std::string& uniformName, glm::vec3 vec) = 0;
        virtual void SetVector2(const std::string& uniformName, glm::vec2 vec) = 0;
    };
};
