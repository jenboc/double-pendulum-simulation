#pragma once

#include "AppSettings.hpp"
#include "DoublePendulum.hpp"

class Application
{
public:
    Application();
    void Run();
private:
    DoublePendulum _pendulum;

    void StartWindow();
    void MainLoop();
    void PhysicsUpdate();
    void Draw();
};
