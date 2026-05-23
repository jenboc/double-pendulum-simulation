#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <map>
#include "Shader.hpp"

namespace Engine::Rendering
{
    class Renderer
    {
        public:
            virtual void StartDrawing() = 0;
            virtual void StopDrawing() = 0;

            virtual void LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) = 0;
            std::shared_ptr<Shader> GetShader(const std::string& name)
            {
                return _shaders.at(name);
            }
            std::shared_ptr<Shader> GetActiveShader()
            {
                return _activeShader;
            }

            void EnableShader(const std::string& shaderName)
            {
                EnableShader(_shaders.at(shaderName));
            }

            virtual void ClearShader() = 0;

            virtual void StartUI() = 0;
            virtual void StopUI() = 0;

            virtual void FillBackground(glm::vec3 colour) = 0;
            virtual void DrawLine(glm::vec2 a, glm::vec2 b, glm::vec3 colour) = 0;
            virtual void DrawRectangle(glm::vec2 c1, glm::vec2 c2, glm::vec3 colour) = 0;
            virtual void DrawCircle(glm::vec2 centre, float radius, glm::vec3 colour) = 0;
        protected:
            virtual void EnableShader(std::shared_ptr<Shader> shader) = 0;
            std::shared_ptr<Shader> _activeShader = nullptr;
            std::map<std::string, std::shared_ptr<Shader>> _shaders;
    };
}
