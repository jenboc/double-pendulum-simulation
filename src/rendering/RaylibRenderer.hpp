#pragma once

#include "Renderer.hpp"
#include "RaylibShader.hpp"
#include <rlgl.h>
#include <raylib.h>
#include <rlImGui.h>

namespace Engine::Rendering
{
    class RaylibRenderer : public Renderer
    {
        public:
            RaylibRenderer(bool darkUi = true)
            {
                rlImGuiSetup(darkUi);
            }

            ~RaylibRenderer()
            {
                rlImGuiShutdown();
            }

            void LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) override
            {
                auto shader = std::make_shared<RaylibShader>(vertexPath, fragmentPath);
                _shaders.insert({ name, shader });
            }

            void ClearShader() override
            {
                EnableShader(nullptr);
            }

            void StartDrawing() override
            {
                BeginDrawing();
            }

            void StopDrawing() override
            {
                EndDrawing();
            }

            void StartUI() override
            {
                rlImGuiBegin();
            }

            void StopUI() override
            {
                rlImGuiEnd();
            }

            void FillBackground(glm::vec3 colour) override
            {
                ClearBackground(ToRaylibColour(colour)); 
            }

            void DrawLine(glm::vec2 a, glm::vec2 b, glm::vec3 colour) override
            {
                Vector2 start { a.x, a.y };
                Vector2 end { b.x, b.y };
                DrawLineV(start, end, ToRaylibColour(colour));   
            }

            void DrawRectangle(glm::vec2 c1, glm::vec2 c2, glm::vec3 colour) override
            {
                glm::vec2 size = c2 - c1;
                DrawRectangleV({ c1.x, c1.y }, { size.x, size.y }, ToRaylibColour(colour));
            }

            void DrawCircle(glm::vec2 centre, float radius, glm::vec3 colour) override
            {
                Vector2 c { centre.x, centre.y };
                DrawCircleV(c, radius, ToRaylibColour(colour));
            }

        protected:
            void EnableShader(std::shared_ptr<Shader> shader) override
            {
                if (_activeShader)
                    _activeShader->Disable();

                rlDrawRenderBatchActive();
                _activeShader = shader;

                if (_activeShader)
                    _activeShader->Enable();
            }

        private:
            static unsigned char ToColourBit(float f)
            {
                f = glm::clamp(f, 0.0f, 1.0f) * 255.0f;
                return static_cast<unsigned char>(f);
            }

            static Color ToRaylibColour(glm::vec3 colour)
            {
                return {
                    ToColourBit(colour.x),
                    ToColourBit(colour.y),
                    ToColourBit(colour.z),
                    255
                };
            }
    };
}
