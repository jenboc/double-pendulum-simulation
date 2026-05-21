#pragma once

#include <raylib.h>
#include <ostream>

Vector4 operator+(Vector4 a, Vector4 b);
Vector4 operator*(float a, Vector4 b);
Vector4& operator+=(Vector4& a, const Vector4& other);

std::ostream& operator<<(std::ostream& os, const Vector4& v);
