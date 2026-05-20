#include <raylib.h>
#include "Application.hpp"

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
   while (!WindowShouldClose())
   {
       Update();
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

void Application::Update()
{
    _pendulum.Update();    
}
