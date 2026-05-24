#pragma once

#include "Renderer.hpp"
#include "RaylibShader.hpp"
#include "RaylibRenderTexture.hpp"
#include <rlgl.h>
#include <raylib.h>
#include <rlImGui.h>

namespace Engine::Rendering
{
    class RaylibRenderer : public Renderer
    {
        public:
            RaylibRenderer(int screenWidth, int screenHeight,
                    bool darkUi = true)
                : _screenWidth(screenWidth), _screenHeight(screenHeight),
                  _sceneTexture(std::make_shared<RaylibRenderTexture>(screenWidth, screenHeight))
            {
                rlImGuiSetup(darkUi);
            }

            ~RaylibRenderer()
            {
                rlImGuiShutdown();
            }

            std::shared_ptr<Texture> LoadTexture(const std::string& path) override
            {
                return std::make_shared<RaylibTexture>(::LoadTexture(path.c_str()));
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

            void StartScene() override
            {
                BeginTextureMode(_sceneTexture->GetRawRenderTexture());
                ClearBackground(BLACK);
            }

            void StopScene() override
            {
                EndTextureMode();
            }

            std::shared_ptr<Texture> GetSceneTexture() override
            {
                return _sceneTexture;
            }

            void DrawScene() override
            {
                Rectangle src {
                    0,
                    0,
                    static_cast<float>(_sceneTexture->GetRawTexture().width),
                    -static_cast<float>(_sceneTexture->GetRawTexture().height)
                };

                Rectangle dst {
                    0,
                    0,
                    static_cast<float>(_screenWidth),
                    static_cast<float>(_screenHeight)
                };

                DrawTexturePro(_sceneTexture->GetRawTexture(), src, dst, {0, 0}, 0.0f,
                        WHITE);
            }

            void DrawTexture(std::shared_ptr<Texture> texture, glm::vec2 src1,
                    glm::vec2 src2, glm::vec2 dst1, glm::vec2 dst2,
                    glm::vec2 origin, float angle, glm::vec3 colour) override
            {
                auto rlTexture = std::static_pointer_cast<RaylibTexture>(texture);

                Rectangle src {
                    src1.x,
                    src1.y,
                    src2.x,
                    src2.y
                };

                Rectangle dst {
                    dst1.x,
                    dst1.y,
                    dst2.x,
                    dst2.y
                };

                Vector2 rlOrigin { origin.x, origin.y };
                Color rlColour = ToRaylibColour(colour);

                DrawTexturePro(rlTexture->GetRawTexture(), src, dst, rlOrigin,
                        angle, rlColour);
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
            int _screenWidth;
            int _screenHeight;
            std::shared_ptr<RaylibRenderTexture> _sceneTexture;

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
