#pragma once

#include <raylib.h>
#include "Shader.hpp"

namespace Engine::Rendering
{
    class RaylibShader : public Shader
    {
    public: 
        RaylibShader(const std::string& vertexPath, const std::string& fragmentPath)
        {
            const char *vert = (vertexPath.length() > 0)
                ? TextFormat(vertexPath.c_str(), GLSL_VERSION)
                : nullptr;

            const char *frag = (fragmentPath.length() > 0)
                ? TextFormat(fragmentPath.c_str(), GLSL_VERSION)
                : nullptr;
            _raylibShader = LoadShader(vert, frag);
        }

        void Enable() override
        {
            BeginShaderMode(_raylibShader);
        }

        void Disable() override
        {
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

        void SetVector2(const std::string& uniformName, glm::vec2 vec) override
        {
            Vector2 raylibVec = { vec.x, vec.y };
            int loc = GetShaderLocation(_raylibShader, uniformName.c_str());
            SetShaderValue(_raylibShader, loc, &raylibVec, SHADER_UNIFORM_VEC2);
        }

    private:
        ::Shader _raylibShader;
    };
};
