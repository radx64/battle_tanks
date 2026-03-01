#pragma once

#include <cmath>
#include "TankSimulator.hpp"

namespace experimental
{

class WaitCondition
{
public:
    virtual bool update(float dt) = 0;
    virtual ~WaitCondition() = default;
};

class WaitSeconds : public WaitCondition 
{
public:
    WaitSeconds(float seconds)
    : remaining_{seconds}
    {}

    bool update(float dt) override
    {
        remaining_ -= dt;
        return remaining_ <= 0.0f;
    }    

protected:
float remaining_;
};

class WaitTurretAligned : public WaitCondition 
{
public:
    WaitTurretAligned(TankSimulator* t, float angle)
    : tank_{t}
    , targetAngle_ {angle}
    {}

    bool update(float) override
    {
        return std::abs(tank_->turretHeading - targetAngle_) < 0.5f;
    }

protected:
    TankSimulator* tank_;
    float targetAngle_;
};

class WaitBodyAligned : public WaitCondition 
{
public:
    WaitBodyAligned(TankSimulator* t, float angle)
    : tank_{t}
    , targetAngle_ {angle}
    {}

    bool update(float) override
    {
        return std::abs(tank_->bodyHeading - targetAngle_) < 0.5f;
    }

protected:
    TankSimulator* tank_;
    float targetAngle_;
};


}  // namespace experimental
