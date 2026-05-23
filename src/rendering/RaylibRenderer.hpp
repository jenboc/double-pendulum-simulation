#pragma once

#include "Renderer.hpp"
#include <raylib.h>
#include <rlImGui.h>

class RaylibRenderer : public Renderer
{
public:
    RaylibRenderer(bool darkUi = true)
    {
        rlImGuiSetup(darkUi);
    }

    ~RaylibRenderer()
    {
        rlImGuiShutdown();
    }

    void StartDrawing() override
    {
        BeginDrawing();
    }

    void StopDrawing() override
    {
        EndDrawing();
    }

    void StartUI() override
    {
        rlImGuiBegin();
    }

    void StopUI() override
    {
        rlImGuiEnd();
    }

    void FillBackground(glm::vec3 colour) override
    {
        ClearBackground(ToRaylibColour(colour)); 
    }

    void DrawLine(glm::vec2 a, glm::vec2 b, glm::vec3 colour) override
    {
        Vector2 start { a.x, a.y };
        Vector2 end { b.x, b.y };
        DrawLineV(start, end, ToRaylibColour(colour));   
    }

    void DrawCircle(glm::vec2 centre, float radius, glm::vec3 colour) override
    {
        Vector2 c { centre.x, centre.y };
        DrawCircleV(c, radius, ToRaylibColour(colour));
    }

private:
    static unsigned char ToColourBit(float f)
    {
        f = glm::clamp(f, 0.0f, 1.0f) * 255.0f;
        return static_cast<unsigned char>(f);
    }

    static Color ToRaylibColour(glm::vec3 colour)
    {
        colour = 255 / static_cast<float>(colour.length()) * colour;
        return {
            ToColourBit(colour.x),
            ToColourBit(colour.y),
            ToColourBit(colour.z),
            255
        };
    }
};
