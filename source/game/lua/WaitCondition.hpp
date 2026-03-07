#pragma once

namespace game::lua
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
    WaitSeconds(float seconds);
    bool update(float dt) override; 

protected:
float remaining_;
};


// TODO: implement rest of of below wait conditions (port from PoC)
// class WaitTurretAligned : public WaitCondition 
// {
// public:
//     WaitTurretAligned(TankSimulator* t, float angle)
//     : tank_{t}
//     , targetAngle_ {angle}
//     {}

//     bool update(float) override
//     {
//         return std::abs(tank_->turretHeading - targetAngle_) < 0.5f;
//     }

// protected:
//     TankSimulator* tank_;
//     float targetAngle_;
// };

// class WaitBodyAligned : public WaitCondition 
// {
// public:
//     WaitBodyAligned(TankSimulator* t, float angle)
//     : tank_{t}
//     , targetAngle_ {angle}
//     {}

//     bool update(float) override
//     {
//         return std::abs(tank_->bodyHeading - targetAngle_) < 0.5f;
//     }

// protected:
//     TankSimulator* tank_;
//     float targetAngle_;
// };


}  // namespace game::lua
