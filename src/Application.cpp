#include <raylib.h>
#include "Application.hpp"

constexpr double PHYSICS_DT = 1.0 / 1000.0;
constexpr double TIME_SCALE = 1.0f;

Application::Application() : _pendulum()
{}

void Application::Run()
{
    StartWindow();
    MainLoop();
    CloseWindow();
}

void Application::StartWindow()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE.c_str());
    SetTargetFPS(60);
}

void Application::MainLoop()
{
    double accumulator = 0.0;

    while (!WindowShouldClose())
    {
        accumulator += TIME_SCALE * GetFrameTime();
        
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
    BeginDrawing();

    ClearBackground(RAYWHITE);
    _pendulum.Draw();

    EndDrawing();
}

void Application::PhysicsUpdate()
{
    _pendulum.Update();    
}
