#include <raylib.h>
#include <imgui.h>
#include "Application.hpp"
#include "FrameTimer.hpp"
#include "AppSettings.hpp"
#include "RaylibInput.hpp"
#include "rendering/RaylibShader.hpp"
#include "rendering/RaylibRenderer.hpp"
#include "rendering/RaylibWindow.hpp"

using namespace Engine::Core;
using namespace Engine::Rendering;

constexpr double PHYSICS_DT = 1.0 / 1000.0;
constexpr double TIME_SCALE = 1.0f;

Application::Application() 
    : _window(std::make_unique<RaylibWindow>(SCREEN_WIDTH, SCREEN_HEIGHT,
                  SCREEN_TITLE)),
      _renderer(std::make_unique<RaylibRenderer>()),
      _input(std::make_unique<RaylibInput>()),
      _pendulum() 
{
    _renderer->LoadShader("background", "", "shaders/background.fs");
}

void Application::Run()
{
    MainLoop();
}

void Application::MainLoop()
{
    FrameTimer timer;
    double accumulator = 0.0;

    while (!_window->ShouldClose())
    {
        accumulator += TIME_SCALE * timer.GetFrameTime();
        
        Update();
        
        while (accumulator >= PHYSICS_DT)
        {
            PhysicsUpdate();
            accumulator -= PHYSICS_DT;
        }

        Draw();
    }
}

void Application::Draw()
{
    auto [centre1, centre2, centre3] = _pendulum.CalculateMassCentres();
    centre1.y = SCREEN_HEIGHT - centre1.y;
    centre2.y = SCREEN_HEIGHT - centre2.y;
    centre3.y = SCREEN_HEIGHT - centre3.y;

    constexpr float lightRadius = 50 * 50;

    _renderer->StartDrawing();

    _renderer->FillBackground({ 1.0f, 1.0f, 1.0f });

    _pendulum.Draw(*_renderer);

    if (_lightsOff)
    {
        _renderer->EnableShader("background");

        auto shader = _renderer->GetActiveShader();
        shader->SetVector2("lights[0].position", centre1);
        shader->SetVector2("lights[1].position", centre2);
        shader->SetVector2("lights[2].position", centre3);
        shader->SetFloat("lights[0].lightRadius", lightRadius / 4);
        shader->SetFloat("lights[1].lightRadius", lightRadius);
        shader->SetFloat("lights[2].lightRadius", lightRadius);

        _renderer->DrawRectangle({ 0.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT },
                { 0.0f, 0.0f, 0.0f });
        _renderer->ClearShader();
    }

    _renderer->StartUI();
    DrawUI();
    _renderer->StopUI();

    _renderer->StopDrawing();
}


float f = 0.0f;
void Application::DrawUI()
{
    auto [l1, l2] = _pendulum.GetLengths();
    auto [m1, m2] = _pendulum.GetMasses();

    constexpr float min_mass = 0.0f;
    constexpr float max_mass = 10.0f;

    constexpr float min_length = 0.1f;
    constexpr float max_length = 10.0f;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::Begin(
            "Pendulum",
            nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_AlwaysAutoResize
    );
    // Mass 1 MUST be non-zero
    ImGui::SliderFloat("Mass 1", &m1, 0.1f, max_mass);
    ImGui::SliderFloat("Mass 2", &m2, min_mass, max_mass);
    ImGui::SliderFloat("Length 1", &l1, min_length, max_length);
    ImGui::SliderFloat("Length 2", &l2, min_length, max_length);
    ImGui::End();
}

void Application::Update()
{
    if (_input->IsSpacePressed())
        _physicsPaused = !_physicsPaused;

    if (_input->IsFPressed())
        _lightsOff = !_lightsOff;

    _pendulum.Update(*_input, _physicsPaused);
}

void Application::PhysicsUpdate()
{
    if (_physicsPaused)
        return;

    _pendulum.PhysicsUpdate(); 
}
