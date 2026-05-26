#pragma once

#include <chrono>

using namespace std::chrono;

namespace Engine::Core
{
    class FrameTimer
    {
        public:
            float GetFrameTime()
            {
                auto now = high_resolution_clock::now();

                if (!_hasTicked)
                {
                    _lastTime = now;
                    _hasTicked = true;
                    return 0.0f;
                }

                duration<float> dt = now - _lastTime;
                _lastTime = now;

                return dt.count();
            }
        private:
            high_resolution_clock::time_point _lastTime { high_resolution_clock::now() };
            bool _hasTicked { false };
    };
}
