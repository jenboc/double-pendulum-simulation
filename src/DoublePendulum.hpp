#pragma once

#include <raylib.h>
#include <memory>
#include "Input.hpp"
#include "rendering/Renderer.hpp"
#include "solving/OdeSolver.hpp"
#include "AppSettings.hpp"

constexpr float CIRCLE_RADIUS = 10;
constexpr float GRAVITY = 9.81;

class DoublePendulum
{
public:
    DoublePendulum();
    DoublePendulum(OdeSolver<glm::vec4> solver, float stepSize);

    std::pair<float&, float&> GetLengths();
    std::pair<float&, float&> GetAngles();
    std::pair<float&, float&> GetMasses();

    void Draw(Renderer& renderer);
    void Update(const Input& input, bool isPhysicsPaused);
    void PhysicsUpdate();
private:
    std::unique_ptr<OdeSolver<glm::vec4>> _solver;
    
    float _originX {static_cast<float>(SCREEN_WIDTH) / 2.0f};
    float _originY {static_cast<float>(SCREEN_HEIGHT) / 2.0f};

    float _length1 { 1.0f };
    float _length2 { 1.0f };
    float _angle1 { 0 };
    float _angle2 { PI / 2 };
    float _mass1 { 2 };
    float _mass2 { 2 };

    // Equation of Motion given as a vector ODE
    glm::vec4 EquationOfMotion(glm::vec4 angles);
    
    // Reset velocity
    void ResetDeltas();

    // Calculate mass centres
    std::tuple<glm::vec2, glm::vec2, glm::vec2> CalculateMassCentres();

    enum class Masses
    {
        Mass1,
        Mass2,
        None
    };
    Masses _selectedMass { Masses::None };
};
