#pragma once

#include <string_view>

namespace experimental
{

struct TankSimulator 
{
    float targetTurretHeading = 0.f;
    float turretHeading = 0.f;
    const float turretRotationSpeed = 600.f;
    bool isTurretRotating = false;

    float targetBodyHeading = 0.f;
    float bodyHeading = 0.f;
    const float bodyRotationSpeed = 400.f;
    bool isBodyRotating = false;

    void processRotation(auto& elementHeading, auto& elementHeadingTarget, auto& isElementRotating, const auto elementRotationSpeed, float dt, std::string_view elementName);
    void update(float dt);
};

}  // namespace experimental
