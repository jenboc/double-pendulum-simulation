#pragma once

namespace Engine::Rendering
{
    class Texture
    {
    public:
        virtual int GetWidth() const = 0;
        virtual int GetHeight() const = 0;
    };
}
