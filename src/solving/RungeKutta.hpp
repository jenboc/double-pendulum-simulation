#pragma once

#include "OdeSolver.hpp"
#include <vector>

// General m-stage implementation
namespace Engine::DESolvers
{
    template <typename T>
        class RungeKutta : public OdeSolver<T>
    {
        public:
            RungeKutta(
                    std::vector<std::vector<float>> a,
                    std::vector<float> b,
                    std::vector<float> c
                    ) : _stageMultipliers(b), _stageStepMultipliers(c), _intermediateStageMultipliers(a)
            {
                // Validate Sizes
                size_t numStages = b.size();

                if (c.size() != numStages - 1)
                    throw std::invalid_argument("Not enough c entries");

                if (a.size() != numStages - 1)
                    throw std::invalid_argument("a needs n-1 subarrays");

                for (size_t i = 0; i < a.size(); i++)
                {
                    if (a[i].size() != i + 1)
                        throw std::invalid_argument("Subarrays of a are malformed");
                }
            }

            T DoStep() override
            {
                if (!this->CanStep())
                    throw std::logic_error("Please ensure step size and f are set");

                std::vector<T> stages(_stageMultipliers.size());
                stages[0] = this->_f(this->_t, this->_lastStep);

                std::vector<float> *a;
                float c;
                float t;
                T intermediateSteps{};
                T x;
                for (size_t i = 1; i < stages.size(); i++)
                {
                    a = &_intermediateStageMultipliers[i - 1];
                    c = _stageStepMultipliers[i - 1];
                    t = this->_t + c * this->_stepSize;

                    intermediateSteps = T{};
                    for (size_t j = 0; j < i; j++)
                    {
                        intermediateSteps += this->_stepSize * (*a)[j] * stages[j];
                    }

                    x = this->_lastStep + intermediateSteps;

                    stages[i] = this->_f(t, x);
                }

                intermediateSteps = T{};
                for (size_t i = 0; i < stages.size(); i++)
                {
                    intermediateSteps += this->_stepSize * _stageMultipliers[i] * stages[i];
                }

                this->_lastStep += intermediateSteps;
                this->_t += this->_stepSize;

                return this->_lastStep;
            }

            static RungeKutta<T> HeunsMethod()
            {
                return RungeKutta<T>(
                        {
                        { 1.0f }
                        },
                        {
                        0.5f, 0.5f
                        },
                        {
                        1.0f
                        }
                        );
            }

            static RungeKutta<T> ForwardEuler()
            {
                return RungeKutta<T>(
                        {},
                        {
                        1.0f
                        },
                        {}
                        );
            }

        private:
            // b1, ..., bm
            std::vector<float> _stageMultipliers;
            // c2, ..., cm
            std::vector<float> _stageStepMultipliers;
            // a21, a31, a32, ..., am1, ..., am(m-1)
            std::vector<std::vector<float>> _intermediateStageMultipliers;
    };
}
