#include <raylib.h>
#include "Application.hpp"
#include "FrameTimer.hpp"
#include "AppSettings.hpp"
#include "RaylibInput.hpp"
#include "rendering/RaylibRenderer.hpp"
#include "rendering/RaylibWindow.hpp"

constexpr double PHYSICS_DT = 1.0 / 1000.0;
constexpr double TIME_SCALE = 1.0f;

Application::Application() 
    : _renderer(std::make_unique<RaylibRenderer>()),
      _window(std::make_unique<RaylibWindow>(SCREEN_WIDTH, SCREEN_HEIGHT,
                  SCREEN_TITLE)),
      _input(std::make_unique<RaylibInput>()),
      _pendulum() 
{
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
        
        Update();
        
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

void Application::Update()
{
    if (_input->IsSpacePressed())
        _physicsPaused = !_physicsPaused;

    if (_input->IsRPressed())
        _pendulum.ResetDeltas();
}

void Application::PhysicsUpdate()
{
    if (_physicsPaused)
        return;

    _pendulum.Update();    
}
