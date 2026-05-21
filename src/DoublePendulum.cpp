#include "DoublePendulum.hpp"
#include "solving/ForwardEuler.hpp"
#include <cmath>


DoublePendulum::DoublePendulum() : _solver(std::make_unique<ForwardEuler<Vector4>>())
{
    std::function<Vector4(float, Vector4)> f = [this](float, Vector4 angles)
    {
        return this->EquationOfMotion(angles);
    };

    _solver->SetStepSize(0.000001f);
    _solver->SetOde(f, { _angle1, _angle2, 0, 0 });
}

DoublePendulum::DoublePendulum(OdeSolver<Vector4> solver, float stepSize)
    : _solver(std::make_unique<OdeSolver<Vector4>>(solver))
{
    std::function<Vector4(float, Vector4)> f = [this](float, Vector4 angles)
    {
        return this->EquationOfMotion(angles);
    };

    _solver->SetStepSize(stepSize);
    _solver->SetOde(f, { _angle1, _angle2, 0, 0 });
}

void DoublePendulum::Draw()
{
    Vector2 center = { _originX, _originY };

    // Circles at origin & ends of pendulums
    // Connect the circles with lines

    DrawCircleV(center, CIRCLE_RADIUS, RED);

    Vector2 nextCenter = { center.x + _length1 * sinf(_angle1),
        center.y + _length1 * cosf(_angle1) };

    DrawCircleV(nextCenter, CIRCLE_RADIUS, GREEN);
    DrawLineV(center, nextCenter, BLACK);

    center = nextCenter;
    nextCenter = { center.x + _length2 * sinf(_angle2),
        center.y + _length2 * cosf(_angle2) };

    DrawCircleV(nextCenter, CIRCLE_RADIUS, BLUE);
    DrawLineV(center, nextCenter, BLACK);
}

#include <iostream>
void DoublePendulum::Update()
{
    Vector4 newAngles = _solver->DoStep();
    std::cout << newAngles.x << ", " << newAngles.y << ", " << newAngles.z << ", " << newAngles.w << std::endl;
    _angle1 = newAngles.x;
    _angle2 = newAngles.y;
}

Vector4 DoublePendulum::EquationOfMotion(Vector4 angles)
{
    // angles = (angle1, angle2, d_angle1, d_angle2)
    float& theta1 = angles.x;
    float& theta2 = angles.y;
    float& theta1Dot = angles.z;
    float& theta2Dot = angles.w;

    float alpha = theta1 - theta2;
    float M = _mass1 + _mass2;

    // theta1DDot Equation
    float numerator = (
        0.5f * _length1 * _mass2 * powf(theta1Dot, 2) * sinf(2 * alpha)
        - _length2 * _mass2 * powf(theta2Dot, 2) * sinf(alpha)
        + GRAVITY * _mass2 * cosf(alpha) * sin(theta2)
        - M * GRAVITY * sinf(theta1)
    );
    float denominator = (
        _length1 * _mass2 * powf(cosf(alpha), 2)
        - M * _length1
    );
    float theta1DDot = -numerator / denominator;
    
    // theta2DDot Equation
    numerator = (
        0.5 * _length2 * _mass2 * powf(theta2Dot, 2) * sinf(2 * alpha)
        - M * _length1 * powf(theta1Dot, 2) * sin(alpha)
        + M * GRAVITY * cosf(alpha) * sinf(theta1)
        - M * GRAVITY * sinf(theta2)
    );
    denominator = (
        _length2 * _mass2 * powf(cosf(alpha), 2)
        - M * _length2
    );
    float theta2DDot = numerator / denominator;

    // Return d/dt (angles)
    return { 
        theta1Dot,
        theta2Dot,
        theta1DDot,
        theta2DDot
    };
}
