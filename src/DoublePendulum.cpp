#include "DoublePendulum.hpp"
#include "solving/ForwardEuler.hpp"
#include "solving/RungeKutta.hpp"
#include <cmath>

constexpr float METRES_TO_PIXELS = 100.0f;

DoublePendulum::DoublePendulum() : _solver(std::make_unique<RungeKutta<glm::vec4>>(RungeKutta<glm::vec4>::HeunsMethod()))
{
    std::function<glm::vec4(float, glm::vec4)> f = [this](float, glm::vec4 angles)
    {
        return this->EquationOfMotion(angles);
    };

    _solver->SetStepSize(0.001f);
    _solver->SetOde(f, { _angle1, _angle2, 0, 0 });
}

DoublePendulum::DoublePendulum(OdeSolver<glm::vec4> solver, float stepSize)
    : _solver(std::make_unique<OdeSolver<glm::vec4>>(solver))
{
    std::function<glm::vec4(float, glm::vec4)> f = [this](float, glm::vec4 angles)
    {
        return this->EquationOfMotion(angles);
    };

    _solver->SetStepSize(stepSize);
    _solver->SetOde(f, { _angle1, _angle2, 0, 0 });
}

void DoublePendulum::ResetDeltas()
{
    _solver->SetStep({ _angle1, _angle2, 0, 0 });
}

void DoublePendulum::Draw(Renderer& renderer)
{
    glm::vec2 centre1 { _originX, _originY };
    glm::vec2 centre2 {
        centre1.x + METRES_TO_PIXELS * _length1 * sinf(_angle1),
        centre1.y - METRES_TO_PIXELS * _length1 * cosf(_angle1)
    };
    glm::vec2 centre3 {
        centre2.x + METRES_TO_PIXELS * _length2 * sinf(_angle2),
        centre2.y - METRES_TO_PIXELS * _length2 * cosf(_angle2)
    };

    glm::vec3 black { 0.0f, 0.0f, 0.0f };
    glm::vec3 green { 0.0f, 1.0f, 0.0f };
    glm::vec3 blue { 0.0f, 0.0f, 1.0f };

    // Draw lines first
    renderer.DrawLine(centre1, centre2, black);
    renderer.DrawLine(centre2, centre3, black);

    // Draw circles
    renderer.DrawCircle(centre1, CIRCLE_RADIUS / 2, black);
    renderer.DrawCircle(centre2, CIRCLE_RADIUS, green);
    renderer.DrawCircle(centre3, CIRCLE_RADIUS, blue);
}

void DoublePendulum::Update()
{
    glm::vec4 newAngles = _solver->DoStep();
    _angle1 = newAngles.x;
    _angle2 = newAngles.y;
}

glm::vec4 DoublePendulum::EquationOfMotion(glm::vec4 angles)
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
