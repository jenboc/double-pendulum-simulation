#pragma once

#include "AppSettings.hpp"
#include "DoublePendulum.hpp"

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
    DoublePendulum _pendulum;

    void MainLoop();
    void PhysicsUpdate();
    void Draw();
};
