#pragma once

#include "Input.hpp"
#include <raylib.h>

class RaylibInput : public Input
{
public:
    bool IsSpacePressed() const override
    {
        return IsKeyPressed(KEY_SPACE);
    }

    bool IsRPressed() const override
    {
        return IsKeyPressed(KEY_R);
    }

    glm::vec2 GetMousePosition() const override
    {
        Vector2 mousePos = ::GetMousePosition();
        return { mousePos.x, mousePos.y };
    }

    bool IsLeftMouseDown() const override
    {
        return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    }
};
