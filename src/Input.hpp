#pragma once

#include <glm/glm.hpp>

namespace Engine::Core
{
    class Input
    {
        public:
            virtual bool IsSpacePressed() const = 0;
            virtual bool IsRPressed() const = 0;
            virtual bool IsFPressed() const = 0;

            virtual glm::vec2 GetMousePosition() const = 0;
            virtual bool IsLeftMouseDown() const = 0;
    };
}
