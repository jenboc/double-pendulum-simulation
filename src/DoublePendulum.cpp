#include "DoublePendulum.hpp"
#include "solving/ForwardEuler.hpp"
#include "solving/RungeKutta.hpp"
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/norm.hpp>

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
    auto [centre1, centre2, centre3] = CalculateMassCentres();
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

void DoublePendulum::Update(const Input& input, bool isPhysicsPaused)
{
    if (input.IsRPressed())
        ResetDeltas();

    // Can only move a ball if physics is paused
    if (!isPhysicsPaused)
        return;

    if (!input.IsLeftMouseDown())
    {
        if (_selectedMass != Masses::None)
        {
            _selectedMass = Masses::None;
            ResetDeltas();
        }
        return;
    }

    const glm::vec2 mousePos = input.GetMousePosition();
    auto [centre1, centre2, centre3] = CalculateMassCentres();

    // None selected => try to select
    constexpr float radius_squared = CIRCLE_RADIUS * CIRCLE_RADIUS;
    if (_selectedMass == Masses::None)
    {
        if (glm::length2(mousePos - centre2) <= radius_squared)
            _selectedMass = Masses::Mass1;
        else if (glm::length2(mousePos - centre3) <= radius_squared)
            _selectedMass = Masses::Mass2;

        return;
    }

    // One is selected => move it
    float& angle = (_selectedMass == Masses::Mass1) ? _angle1 : _angle2;
    const glm::vec2& pivot = (_selectedMass == Masses::Mass1) ? centre1 : centre2;

    glm::vec2 pivotToMouse = glm::normalize(mousePos - pivot);
    angle = atan2f(pivotToMouse.x, -pivotToMouse.y);
}

void DoublePendulum::PhysicsUpdate()
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

std::tuple<glm::vec2, glm::vec2, glm::vec2> DoublePendulum::CalculateMassCentres()
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

    return { centre1, centre2, centre3 };
}

std::pair<float&, float&> DoublePendulum::GetLengths()
{
    return { _length1, _length2 };
}

std::pair<float&, float&> DoublePendulum::GetAngles()
{
    return { _angle1, _angle2 };
}

std::pair<float&, float&> DoublePendulum::GetMasses()
{
    return { _mass1, _mass2 };
}
