#pragma once

#include <raylib.h>
#include "OdeSolver.hpp"

namespace Engine::DESolvers
{
    template <typename T>
    class ForwardEuler : public OdeSolver<T>
    {
        public:
            T DoStep() override
            {
                if (!this->CanStep())
                    throw std::logic_error("Please ensure step size and f are set");

                this->_lastStep = this->_lastStep + this->_f(this->_t,
                        this->_lastStep);
                this->_t += this->_stepSize;

                return this->_lastStep;
            }
    };
}
