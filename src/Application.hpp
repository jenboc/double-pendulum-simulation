#pragma once

#include "AppSettings.hpp"
#include "DoublePendulum.hpp"
#include "Input.hpp"

#include "rendering/Window.hpp"
#include "rendering/Renderer.hpp"

#include <memory>

namespace Engine::Core
{
    class Application
    {
        public:
            Application();
            void Run();
        private:
            std::unique_ptr<Rendering::Window> _window;
            std::unique_ptr<Rendering::Renderer> _renderer;
            std::unique_ptr<Input> _input;
 
            std::shared_ptr<Engine::Rendering::Texture> _backgroundTexture;
            DoublePendulum _pendulum;
            bool _physicsPaused { false };
            bool _lightsOff { true };

            void MainLoop();
            void Update();
            void PhysicsUpdate();
            void Draw();
            void DrawUI();
    };
}
