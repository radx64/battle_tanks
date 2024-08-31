#include "engine/CameraController.hpp"
#include "engine/Camera.hpp"

namespace engine
{

constexpr float CAMERA_SPEED = 1000.f;

CameraController::CameraController (Camera* camera)
: camera_{camera}
{}

void CameraController::onKeyPressed(const sf::Keyboard::Key key)
{
    switch(key)
    {
        case sf::Keyboard::W : camera_->moveY(-CAMERA_SPEED); break;
        case sf::Keyboard::S : camera_->moveY(CAMERA_SPEED); break;
        case sf::Keyboard::A : camera_->moveX(-CAMERA_SPEED); break;
        case sf::Keyboard::D : camera_->moveX(CAMERA_SPEED); break;
        default : break;
    }
}

void CameraController::onKeyReleased(const sf::Keyboard::Key key)
{
    switch(key)
    {
        case sf::Keyboard::W : camera_->moveY(0.f); break;
        case sf::Keyboard::S : camera_->moveY(0.f); break;
        case sf::Keyboard::A : camera_->moveX(0.f); break;
        case sf::Keyboard::D : camera_->moveX(0.f); break;
        default : break;
    }

}

}  // namespace engine
