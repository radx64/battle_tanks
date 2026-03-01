
#include "TankSimulator.hpp"

#include <cmath>
#include <iostream>

namespace experimental
{

void TankSimulator::processRotation(auto& elementHeading, auto& elementHeadingTarget, auto& isElementRotating, const auto elementRotationSpeed, float dt, std::string_view elementName)
{
    if (not isElementRotating)
    {
        return;
    }

    std::cout << "Rotating " << elementName << " current: " << elementHeading << " target: " << elementHeadingTarget << std::endl;
    
    float diff = elementHeadingTarget - elementHeading;
    float step = elementRotationSpeed * dt;
    if (std::abs(diff) <= step) 
    {
        elementHeading = elementHeadingTarget;
        isElementRotating = false;
    } 
    else 
    {
        elementHeading += (diff > 0 ? step : -step);
    }
}

void TankSimulator::update(float dt)
{
    processRotation(turretHeading, targetTurretHeading, isTurretRotating, turretRotationSpeed, dt, "turret");
    processRotation(bodyHeading, targetBodyHeading, isBodyRotating, bodyRotationSpeed, dt, "body");
}


}  // namespace experimental
