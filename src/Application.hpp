#pragma once

#include "AppSettings.hpp"
#include "DoublePendulum.hpp"
#include "Input.hpp"

#include "rendering/Window.hpp"
#include "rendering/Renderer.hpp"

#include <memory>

class Application
{
public:
    Application();
    void Run();
private:
    std::unique_ptr<Renderer> _renderer;
    std::unique_ptr<Window> _window;
    std::unique_ptr<Input> _input;
    DoublePendulum _pendulum;
    bool _physicsPaused { false };


    void MainLoop();
    void Update();
    void PhysicsUpdate();
    void Draw();
};
