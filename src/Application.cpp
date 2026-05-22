#include <raylib.h>
#include "Application.hpp"
#include "FrameTimer.hpp"
#include "AppSettings.hpp"
#include "rendering/RaylibRenderer.hpp"
#include "rendering/RaylibWindow.hpp"

constexpr double PHYSICS_DT = 1.0 / 1000.0;
constexpr double TIME_SCALE = 1.0f;

Application::Application() : _pendulum()
{
    _window = std::make_unique<RaylibWindow>(SCREEN_WIDTH, SCREEN_HEIGHT,
            SCREEN_TITLE);
    _renderer = std::make_unique<RaylibRenderer>();
}

void Application::Run()
{
    MainLoop();
}

void Application::MainLoop()
{
    FrameTimer timer;
    double accumulator = 0.0;

    while (!_window->ShouldClose())
    {
        accumulator += TIME_SCALE * timer.GetFrameTime();
        
        while (accumulator >= PHYSICS_DT)
        {
            PhysicsUpdate();
            accumulator -= PHYSICS_DT;
        }

        Draw();
    }
}

void Application::Draw()
{
    _renderer->StartDrawing();

    _renderer->FillBackground(glm::vec3 { 1.0f, 1.0f, 1.0f });
    _pendulum.Draw(*_renderer);

    _renderer->StopDrawing();
}

void Application::PhysicsUpdate()
{
    _pendulum.Update();    
}
