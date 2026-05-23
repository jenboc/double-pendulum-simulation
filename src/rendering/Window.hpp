#pragma once

namespace Engine::Rendering
{
    class Window
    {
        public:
            virtual bool ShouldClose() = 0;
            virtual void PollEvents() = 0;
            virtual void SwapBuffers() = 0;
            virtual int GetWidth() = 0;
            virtual int GetHeight() = 0;
    };
};
