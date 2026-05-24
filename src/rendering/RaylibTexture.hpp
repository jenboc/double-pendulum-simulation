#pragma once

#include <stdexcept>
#include <raylib.h>
#include "Texture.hpp"

namespace Engine::Rendering
{
    class RaylibTexture : public Texture
    {
    public:
        RaylibTexture()
        {}

        RaylibTexture(Texture2D tex) : _tex(tex)
        {
            if (tex.id == 0)
            {
                throw std::invalid_argument("Failed to load texture");
            }
        }

        virtual int GetWidth() const override
        {
            return _tex.width;
        }

        virtual int GetHeight() const override
        {
            return _tex.height;
        }

        virtual Texture2D& GetRawTexture()
        {
            return _tex;
        }
    protected:
        Texture2D _tex;
    };
}
