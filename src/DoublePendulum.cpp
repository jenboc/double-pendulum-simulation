#include "DoublePendulum.hpp"
#include "solving/ForwardEuler.hpp"
#include "solving/RungeKutta.hpp"
#include <cmath>

constexpr float METRES_TO_PIXELS = 100.0f;

DoublePendulum::DoublePendulum() : _solver(std::make_unique<RungeKutta<Vector4>>(RungeKutta<Vector4>::HeunsMethod()))
{
    std::function<Vector4(float, Vector4)> f = [this](float, Vector4 angles)
    {
        return this->EquationOfMotion(angles);
    };

    _solver->SetStepSize(0.001f);
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
    Vector2 centre1 = { _originX, _originY };
    Vector2 centre2 = {
        centre1.x + METRES_TO_PIXELS * _length1 * sinf(_angle1),
        centre1.y - METRES_TO_PIXELS * _length1 * cosf(_angle1)
    };
    Vector2 centre3 = {
        centre2.x + METRES_TO_PIXELS * _length2 * sinf(_angle2),
        centre2.y - METRES_TO_PIXELS * _length2 * cosf(_angle2)
    };

    // Draw lines first
    DrawLineV(centre1, centre2, BLACK);
    DrawLineV(centre2, centre3, BLACK);

    // Draw circles
    DrawCircleV(centre1, CIRCLE_RADIUS, RED);
    DrawCircleV(centre2, CIRCLE_RADIUS, GREEN);
    DrawCircleV(centre3, CIRCLE_RADIUS, BLUE);
}

void DoublePendulum::Update()
{
    Vector4 newAngles = _solver->DoStep();
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

    float gap = theta2 - theta1;

    float denominator = _length1 * _mass2 * cosf(2 * gap)
        - 2 * _length1 * _mass1
        - _length1 * _mass2;
    float numerator = 2 * _length2 * _mass2 * powf(theta2Dot, 2) * sinf(gap)
        + _length1 * _mass2 * powf(theta1Dot, 2) * sinf(2 * gap)
        - GRAVITY * _mass2 * sinf(2 * theta2 - theta1)
        + (2 * GRAVITY * _mass1 + GRAVITY * _mass2) * sinf(theta1);
    float theta1DDot = -numerator / denominator;

    denominator = _length2 * _mass2 * cosf(2 * gap)
        - 2 * _length2 * _mass1
        - _length2 * _mass2;
    numerator = _length2 * _mass2 * powf(theta2Dot, 2) * sinf(2 * gap)
        + 2 * (_length1 * _mass1 + _length1 * _mass2) * powf(theta1Dot, 2) * sinf(gap)
        - (GRAVITY * _mass1 + GRAVITY * _mass2) * sinf(theta2 - 2 * theta1)
        - (GRAVITY * _mass1 + GRAVITY * _mass2) * sinf(theta2);
    float theta2DDot = numerator / denominator;

    return { theta1Dot, theta2Dot, theta1DDot, theta2DDot };
}
