cmake_minimum_required(VERSION 3.16)

project(double-pendulum
    VERSION 1.0
    LANGUAGES CXX
)

# ----------------------------------------
# C++ Standard
# ----------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ----------------------------------------
# Executable
# ----------------------------------------
add_executable(double-pendulum
    src/main.cpp
    src/Application.cpp
    src/DoublePendulum.cpp
    src/solving/VectorExtensions.cpp
)

# ----------------------------------------
# Find required packages on system
# ----------------------------------------

find_package(raylib REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(double-pendulum PRIVATE raylib glm::glm)

# ----------------------------------------
# Compiler warnings
# ----------------------------------------
if (MSVC)
    target_compile_options(double-pendulum PRIVATE /W4)
else()
    target_compile_options(double-pendulum PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ----------------------------------------
# macOS OpenGL dependencies
# ----------------------------------------
if (APPLE)
    target_link_libraries(double-pendulum PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif()
