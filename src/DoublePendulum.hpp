#pragma once

#include <raylib.h>
#include <memory>
#include "solving/OdeSolver.hpp"
#include "AppSettings.hpp"

constexpr float CIRCLE_RADIUS = 10;

class DoublePendulum
{
public:
    DoublePendulum();
    DoublePendulum(OdeSolver<Vector2> solver, float stepSize);
    void Draw();
    void Update();
private:
    std::unique_ptr<OdeSolver<Vector2>> _solver;
    
    float _originX {static_cast<float>(SCREEN_WIDTH) / 2.0f};
    float _originY {static_cast<float>(SCREEN_HEIGHT) / 2.0f};

    float _length1 { 100.0f };
    float _length2 { 100.0f };
    float _angle1 { 0 };
    float _angle2 { 0 };
    float _mass1 { 1 };
    float _mass2 { 1 };

    // Equation of Motion given as a vector ODE
    Vector2 EquationOfMotion(float t, Vector2 angles);
};
