#pragma once

#include "Input.hpp"
#include <raylib.h>

class RaylibInput : public Input
{
public:
    bool IsSpacePressed() override
    {
        return IsKeyPressed(KEY_SPACE);
    }

    bool IsRPressed() override
    {
        return IsKeyPressed(KEY_R);
    }
};
