#pragma once

#include "engine/input/KeyboardReceiver.hpp"

namespace engine {class Camera;}

namespace engine 
{

class CameraController : public engine::input::KeyboardReceiver
{
public:
    CameraController (Camera* camera);

protected:
    void onKeyPressed(const sf::Event::KeyEvent& keyEvent) override;
    void onKeyReleased(const sf::Event::KeyEvent& keyEvent) override;

    Camera* camera_;
};
}  // namespace engine
