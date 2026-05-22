#pragma once

#include "Window.hpp"
#include <raylib.h>
#include <string>

class RaylibWindow : public Window
{
public:
    RaylibWindow(int width, int height, const std::string& title, int fps = 60)
    {
        InitWindow(width, height, title.c_str());
        SetTargetFPS(fps);
    }

    ~RaylibWindow()
    {
        CloseWindow();
    }

    bool ShouldClose() override
    {
        return WindowShouldClose();
    }

    void PollEvents() override
    {
        // Handled by raylib
    }

    void SwapBuffers() override
    {
        // Handled by raylib
    }

    int GetWidth() override
    {
        return GetScreenWidth();
    }

    int GetHeight() override
    {
        return GetScreenHeight();
    }
};
