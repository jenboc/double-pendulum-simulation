#pragma once

#include "Input.hpp"
#include <raylib.h>

namespace Engine::Core
{
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

            bool IsFPressed() const override
            {
                return IsKeyPressed(KEY_F);
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
}
