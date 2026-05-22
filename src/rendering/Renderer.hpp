#pragma once

#include <glm/glm.hpp>

class Renderer
{
public:
    virtual void StartDrawing() = 0;
    virtual void StopDrawing() = 0;
    virtual void FillBackground(glm::vec3 colour) = 0;
    virtual void DrawLine(glm::vec2 a, glm::vec2 b, glm::vec3 colour) = 0;
    virtual void DrawCircle(glm::vec2 centre, float radius, glm::vec3 colour) = 0;
};
