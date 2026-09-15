#include "engine/CameraController.hpp"
#include "engine/Camera.hpp"

namespace engine
{

constexpr float CAMERA_SPEED = 1000.f;

CameraController::CameraController (Camera* camera)
: camera_{camera}
{}

void CameraController::onKeyPressed(const engine::events::KeyPressed& keyEvent)
{
    switch(keyEvent.getKeyCode())
    {
        case engine::core::KeyCode::W : camera_->moveY(-CAMERA_SPEED); break;
        case engine::core::KeyCode::S : camera_->moveY(CAMERA_SPEED); break;
        case engine::core::KeyCode::A : camera_->moveX(-CAMERA_SPEED); break;
        case engine::core::KeyCode::D : camera_->moveX(CAMERA_SPEED); break;
        default : break;
    }
}

void CameraController::onKeyReleased(const engine::events::KeyReleased& keyEvent)
{
    switch(keyEvent.getKeyCode())
    {
        case engine::core::KeyCode::W : camera_->moveY(0.f); break;
        case engine::core::KeyCode::S : camera_->moveY(0.f); break;
        case engine::core::KeyCode::A : camera_->moveX(0.f); break;
        case engine::core::KeyCode::D : camera_->moveX(0.f); break;
        default : break;
    }

}

}  // namespace engine
