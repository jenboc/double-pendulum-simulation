#pragma once

#include <functional>
#include <stdexcept>

template <typename T>
class OdeSolver
{
public:
    void SetStepSize(float stepSize)
    {
        _stepSize = stepSize;
    }
    void SetOde(std::function<T(float, T)> f, T initial)
    {
        _f = f;
        _t = 0;
        _lastStep = initial;
    }
    virtual T DoStep()
    {
        throw std::logic_error("Not Implemented");
    }
protected:
    float _stepSize { 0.0f };
    
    // y' = f(t, y)
    std::function<T(float, T)> _f;
    float _t;
    T _lastStep;

    bool CanStep()
    {
        return _stepSize > 0.0f && _f;
    }
};
