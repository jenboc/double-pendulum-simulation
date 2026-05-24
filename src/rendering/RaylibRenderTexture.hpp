#pragma once

#include <raylib.h>
#include "RaylibTexture.hpp"

namespace Engine::Rendering
{
    class RaylibRenderTexture : public RaylibTexture
    {
    public:
        RaylibRenderTexture(int w, int h) : _rt(LoadRenderTexture(w, h))
        {}

        ~RaylibRenderTexture()
        {
            UnloadRenderTexture(_rt);
        }

        int GetWidth() const override
        {
            return _rt.texture.width;
        }

        int GetHeight() const override
        {
            return _rt.texture.height;
        }

        Texture2D& GetRawTexture() override
        {
            return _rt.texture;
        }

        RenderTexture2D& GetRawRenderTexture()
        {
            return _rt;
        }

    private:
        RenderTexture2D _rt;
    };
}
