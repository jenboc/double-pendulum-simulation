# Double Pendulum Simulation

A real-time 2D double pendulum simulation written in C++ using [Raylib](https://www.raylib.com/). The project explores the numerical solution of ordinary differential equations (ODEs), Lagrangian mechanics, and software architecture.

The equations of motion were derived symbolically using SageMath and implemented in C++. The simulation uses a general-purpose Runge–Kutta solver to numerically integrate the resulting system of ODEs.

## Overview

The double pendulum is a classical mechanical system exhibiting chaotic behaviour, where small differences in initial conditions can lead to significantly different trajectories.

The primary objective of this project was to explore the mathematical and computational aspects of simulating such a system, alongside experimenting with C++ application architecture and graphics programming.

### Features

- Real-time 2D double pendulum simulation.
- Equations of motion derived using Lagrangian mechanics and SageMath's symbolic algebra.
- Generic ODE solver architecture supporting multiple numerical integration methods.
- Implementations of Forward Euler and general explicit Runge–Kutta methods.
- Heun's method used for the current simulation.
- Configurable pendulum masses, lengths, and colours through an ImGui interface.
- Interactive repositioning of the pendulum masses to explore different initial conditions.
- Custom GLSL fragment shader providing lighting effects.
- Framework-agnostic application architecture, with Raylib-specific implementations of rendering interfaces.

## Mathematical Background

The equations of motion were derived using Lagrangian mechanics, applying the Euler–Lagrange equations to the system's kinetic and potential energies.

The resulting equations describe the angular positions of the two pendulum arms, \(\theta_1\) and \(\theta_2\).

These equations were derived and simplified using SageMath's symbolic algebra capabilities before being translated into C++.

The resulting system of second-order ODEs is reorganised into a first-order vector ODE of the form

```math
\frac{d\mathbf{y}}{dt} = f(t,\mathbf{y})
```

where the state vector is represented by a `glm::vec4`:

```math
\mathbf{y} =
\begin{pmatrix}
\theta_1 \\
\theta_2 \\
\dot{\theta}_1 \\
\dot{\theta}_2
\end{pmatrix}
```

The full derivation is available in the accompanying Jupyter Notebook:

[DerivationNotebook.ipynb](./docs/DerivationNotebook.ipynb)

### Physical Assumptions

The simulation assumes an ideal double pendulum:

- The pendulum rods are rigid and massless.
- The masses are treated as point masses.
- There is no air resistance or damping.
- No external forces are applied.
- Gravitational acceleration is constant.

The user can modify the two masses and rod lengths through the ImGui interface.

## Numerical Methods

The numerical integration methods are implemented independently of the double pendulum equations. This allows the same solvers to be used with other systems of ordinary differential equations.

### ODE Solver Architecture

A templated base class, `OdeSolver<T>`, defines the common interface for numerical integration methods.

The ODE is supplied as a function:

```math
\mathbf{y}' = f(t,\mathbf{y})
```

Concrete implementations inherit from this base class and implement their respective integration schemes through `DoStep()`.

Currently implemented methods:

- **Forward Euler:** A first-order explicit numerical integration method.
- **Runge–Kutta:** A general explicit Runge–Kutta implementation.

### General Runge–Kutta Implementation

The Runge–Kutta solver is designed around the Butcher tableau, accepting the coefficient arrays \(a\), \(b\), and \(c\) as constructor arguments.

This allows different explicit Runge–Kutta methods to be implemented using the same solver, without having to rewrite the underlying integration algorithm.

The simulation currently uses **Heun's method**, a second-order explicit Runge–Kutta method, through this general implementation.

### Physics Update Loop

The simulation uses a fixed physics timestep of

```math
\Delta t = 0.001 \text{ seconds}
```

Physics updates are decoupled from rendering using an accumulator-based update loop. This allows the simulation to perform multiple physics updates when necessary, independently of the rendering frame rate.

Conceptually, the main loop follows this structure:

1. Accumulate elapsed frame time.
2. Update application state and handle input.
3. Perform physics updates while the accumulated time exceeds the physics timestep.
4. Render the current state.

This helps maintain consistent numerical integration regardless of variations in rendering performance.

## Controls

| Input | Action |
|---|---|
| `SPACE` | Pause or resume the simulation. |
| `R` | Restart the simulation using the current mass positions as initial conditions. |
| `F` | Toggle shader lighting effects. |
| Mouse dragging | Reposition the masses while the simulation is paused. |

When the simulation is paused, the masses can be repositioned to explore different initial conditions. Resuming after repositioning automatically restarts the simulation using the modified positions.

Resuming without modifying the positions continues the existing simulation state.

The ImGui interface additionally allows the user to modify:
- Masses \(m_1\) and \(m_2\).
- Pendulum lengths \(l_1\) and \(l_2\).
- Colours of the pendulum masses.

## Technical Implementation

### Language and Libraries

| Technology | Purpose |
|---|---|
| C++ | Core application and simulation logic |
| [Raylib](https://www.raylib.com/) | Graphics, window management and rendering |
| [GLM](https://github.com/g-truc/glm) | Vector and mathematical operations |
| [Dear ImGui](https://github.com/ocornut/imgui) | Interactive user interface |
| [rlImGui](https://github.com/raylib-extras/rlImGui) | Raylib and ImGui integration |
| GLSL | Custom fragment shader |
| [CMake](https://cmake.org/) | Build system |
| [SageMath](https://www.sagemath.org/) | Symbolic derivation of the equations of motion |

### Application Architecture

The application follows a framework-agnostic architecture, separating the core application logic from the graphics framework.

Abstract interfaces, such as `Window`, `Texture`, and `Renderer`, define the functionality required by the core application.

Raylib-specific implementations provide the underlying functionality by calling the corresponding Raylib functions.

This approach keeps the core application independent of Raylib and makes it possible to replace the rendering backend without fundamentally changing the application logic.

### Custom Shader

A custom GLSL fragment shader (`lighting.fs`) provides a coloured lighting effect around the pendulum masses.

The effect can be enabled or disabled during runtime.

## Building

### Requirements

The project has been tested on Linux.

Requirements:

- CMake 3.16 or newer.
- A C++ compiler supporting the language features used by the project.
- Raylib (installed on the system).
- GLM (installed on the system).
- Git (for cloning the repository and its submodules).

Dear ImGui and rlImGui are included as Git submodules in the `external/` directory.

Additional system dependencies may be required depending on the Linux distribution and the installed Raylib configuration.

### Clone the Repository

Clone the repository and initialise its submodules:

```bash
git clone --recurse-submodules <repository-url>
cd double-pendulum
```

Alternatively, if the repository has already been cloned:

```bash
git submodule update --init --recursive
```

### Configure and Build

Configure the project using CMake:

```bash
cmake -B build
```

Build the executable:

```bash
cmake --build build
```

### Running

Run the resulting executable from the repository root directory.

```bash
./build/<executable-name>
```

The working directory matters because the application loads resources such as shaders and image assets using relative paths.

## Project Structure
- `src/`: Core application logic and implementation.
- `src/rendering/`: Rendering abstractions and Raylib-specific implementations.
- `src/solving/`: ODE solver implementations, including Forward Euler and Runge–Kutta.
- `external/`: Third-party dependencies included as Git submodules.
- `shaders/`: Custom GLSL shaders.
- `docs/`: Accompanying documents, e.g. the symbolic derivation of the equations of motion.
- `res/`: Non-code files used in the simulation, e.g. the background image used in the simulation.

## Future Improvements

Some potential areas for further development include:

- Adding runtime selection between numerical integration methods.
- Comparing the numerical behaviour of different integration schemes.
- Investigating numerical energy conservation and integration stability.
- Adding trajectory visualisation and phase-space plots.
- Extending the simulation with additional numerical integration methods.

## Licence

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) for more details.
