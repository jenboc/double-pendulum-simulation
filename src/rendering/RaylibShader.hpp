#pragma once

#include <raylib.h>
#include "Shader.hpp"
#include <iostream>

namespace Engine::Rendering
{
    class RaylibShader : public Shader
    {
    public: 
        RaylibShader(const std::string& vertexPath, const std::string& fragmentPath)
        {
            _raylibShader = LoadShader(vertexPath.c_str(), fragmentPath.c_str());
        }

        void Enable() override
        {
            std::cout << "Enabled" << std::endl;
            BeginShaderMode(_raylibShader);
        }

        void Disable() override
        {
            std::cout << "Disabled" << std::endl;
            EndShaderMode();
        }

        void SetFloat(const std::string& uniformName, float f) override
        {
            int loc = GetShaderLocation(_raylibShader, uniformName.c_str());
            SetShaderValue(_raylibShader, loc, &f, SHADER_UNIFORM_FLOAT);
        }

        void SetVector3(const std::string& uniformName, glm::vec3 vec) override
        {
            Vector3 raylibVec = { vec.x, vec.y, vec.z };
            int loc = GetShaderLocation(_raylibShader, uniformName.c_str());
            SetShaderValue(_raylibShader, loc, &raylibVec, SHADER_UNIFORM_VEC3);
        }
    private:
        ::Shader _raylibShader;
    };
};
