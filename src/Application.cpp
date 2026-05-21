#include <raylib.h>
#include "Application.hpp"

constexpr double PHYSICS_DT = 1.0 / 100.0;

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
        accumulator += GetFrameTime();
        
        if (accumulator >= PHYSICS_DT)
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
