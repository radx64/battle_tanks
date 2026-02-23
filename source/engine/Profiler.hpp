#pragma once

#include <cassert>

#include <SFML/System.hpp>

#include "engine/math/Math.hpp"

namespace engine
{

class Profiler
{
public:
    Profiler(sf::Clock& clock, std::string_view name);

    void startFrame();
    void endFrame();
    int32_t getLastFrame() const;
    int32_t getLastAverage() const;

protected:
    std::string name_;
    sf::Time frameStartTime_{};
    sf::Time frameEndTime_{};

    sf::Clock& clock_;
    math::Average average_;
    int32_t lastFrame_{};
    int32_t lastAverage_{};
};

}  // engine
