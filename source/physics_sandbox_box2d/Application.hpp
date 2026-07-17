#pragma once

#include "engine/Application.hpp"
#include "MouseController.hpp"

#include <box2d/box2d.h>

namespace sandbox
{
class Application : public engine::Application
{
public:
    Application();

protected:
    void spawnCircle(float x, float y, float radius);
    void spawnSquare(float x, float y, float side);
    void onInit() override;
    void onClose() override;
    void onEvent(const sf::Event& event) override;
    void onUpdate(float timeStep) override;
    void onRender() override; 

    MouseController mouseController_;

    b2WorldId worldId_;
    std::vector<b2BodyId> bodies_;
};

}  // namespace sandbox
