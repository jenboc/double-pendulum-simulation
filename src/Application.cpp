#include <raylib.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
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
      _renderer(std::make_unique<RaylibRenderer>(SCREEN_WIDTH, SCREEN_HEIGHT)),
      _input(std::make_unique<RaylibInput>()),
      _backgroundTexture(_renderer->LoadTexture("background.png")),
      _pendulum() 
{
    _renderer->LoadShader("lighting", "", "shaders/lighting.fs");
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
    auto [colour1, colour2] = _pendulum.GetColours();
 
    constexpr float lightRadius = 50;

    _renderer->StartDrawing();

    _renderer->StartScene();
        _renderer->DrawTexture(_backgroundTexture, { 0, 0 }, { SCREEN_WIDTH, SCREEN_HEIGHT },
                { 0, 0 }, { _backgroundTexture->GetWidth(), _backgroundTexture->GetHeight() });

        _pendulum.Draw(*_renderer);
    _renderer->StopScene();
   
    _renderer->DrawScene();

    if (_lightsOff)
    {
        _renderer->EnableShader("lighting");

        auto shader = _renderer->GetActiveShader();
        shader->SetVector2("lights[0].position", centre2);
        shader->SetVector2("lights[1].position", centre3);
        shader->SetVector3("lights[0].colour", colour1);
        shader->SetVector3("lights[1].colour", colour2);
 
        shader->SetFloat("lights[0].radius", lightRadius);
        shader->SetFloat("lights[1].radius", lightRadius);

        shader->SetTexture("sceneTexture", _renderer->GetSceneTexture());
    
        _renderer->DrawTexture(_backgroundTexture, { 0, 0 }, { SCREEN_WIDTH, SCREEN_HEIGHT },
                { 0, 0 }, { _backgroundTexture->GetWidth(), _backgroundTexture->GetHeight() });

        _renderer->ClearShader();
    }

    _renderer->StartUI();
    DrawUI();
    _renderer->StopUI();

    _renderer->StopDrawing();
}

void Application::DrawUI()
{
    auto [l1, l2] = _pendulum.GetLengths();
    auto [m1, m2] = _pendulum.GetMasses();
    auto [c1, c2] = _pendulum.GetColours();

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
    ImGui::ColorEdit3("Colour 1", glm::value_ptr(c1));
    ImGui::ColorEdit3("Colour 2", glm::value_ptr(c2));

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
