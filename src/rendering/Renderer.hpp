#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <map>
#include "Shader.hpp"
#include "Texture.hpp"

namespace Engine::Rendering
{
    class Renderer
    {
        public:
            virtual void StartDrawing() = 0;
            virtual void StopDrawing() = 0;

            virtual void StartScene() = 0;
            virtual void StopScene() = 0;
            virtual void DrawScene() = 0;

            virtual std::shared_ptr<Texture> GetSceneTexture() = 0;
            virtual std::shared_ptr<Texture> LoadTexture(const std::string& path) = 0;

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
            virtual void DrawTexture(std::shared_ptr<Texture> texture, glm::vec2 src1,
                    glm::vec2 src2, glm::vec2 dst1, glm::vec2 dst2,
                    glm::vec2 origin = {0, 0}, float angle = 0.0f,
                    glm::vec3 colour = {1, 1, 1}) = 0;
            virtual void DrawRectangle(glm::vec2 c1, glm::vec2 c2, glm::vec3 colour) = 0;
            virtual void DrawCircle(glm::vec2 centre, float radius, glm::vec3 colour) = 0;
        protected:
            virtual void EnableShader(std::shared_ptr<Shader> shader) = 0;
            std::shared_ptr<Shader> _activeShader = nullptr;
            std::map<std::string, std::shared_ptr<Shader>> _shaders;
    };
}
