#pragma once

#include <raylib.h>
#include "OdeSolver.hpp"

template <typename T>
class ForwardEuler : public OdeSolver<T>
{
public:
    T DoStep() override
    {
        if (!this->CanStep())
            throw std::logic_error("Please ensure step size and f are set");

        this->_t += this->_stepSize;
        this->_lastStep = this->_lastStep + this->_f(this->_t,
                this->_lastStep);

        return this->_lastStep;
    }
};

Vector4 operator+(Vector4 a, Vector4 b)
{
    return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}
