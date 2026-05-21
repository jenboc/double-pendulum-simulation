#include "VectorExtensions.hpp"

Vector4 operator+(Vector4 a, Vector4 b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

Vector4 operator*(float a, Vector4 b)
{
    return { a * b.x, a * b.y, a * b.z, a * b.w };
}

Vector4& operator+=(Vector4& a, const Vector4& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    a.w += b.w;
    return a;
}

std::ostream& operator<<(std::ostream& os, const Vector4& v)
{
    os << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
    return os;
}
