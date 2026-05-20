#include "DoublePendulum.hpp"
#include "solving/ForwardEuler.hpp"
#include <cmath>

DoublePendulum::DoublePendulum() : _solver(std::make_unique<ForwardEuler<Vector2>>())
{
    std::function<Vector2(float, Vector2)> f = [this](float t, Vector2 angles)
    {
        return this->EquationOfMotion(t, angles);
    };

    _solver->SetStepSize(0.001f);
    _solver->SetOde(f, { _angle1, _angle2 });
}

DoublePendulum::DoublePendulum(OdeSolver<Vector2> solver, float stepSize)
    : _solver(std::make_unique<OdeSolver<Vector2>>(solver))
{
    std::function<Vector2(float, Vector2)> f = [this](float t, Vector2 angles)
    {
        return this->EquationOfMotion(t, angles);
    };

    _solver->SetStepSize(stepSize);
    _solver->SetOde(f, { _angle1, _angle2 });
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

void DoublePendulum::Update()
{
    Vector2 newAngles = _solver->DoStep();
    _angle1 = newAngles.x;
    _angle2 = newAngles.y;
}

Vector2 DoublePendulum::EquationOfMotion(float t, Vector2 angles)
{
    return { GetFrameTime(), -GetFrameTime() };
}
