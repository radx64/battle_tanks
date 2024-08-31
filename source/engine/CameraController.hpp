#include "engine/input/KeyboardReceiver.hpp"

namespace engine {class Camera;}

namespace engine 
{

class CameraController : public engine::input::KeyboardReceiver
{
public:
    CameraController (Camera* camera);

protected:
    void onKeyPressed(const sf::Keyboard::Key key) override;
    void onKeyReleased(const sf::Keyboard::Key key) override;

    Camera* camera_;
};
}  // namespace engine
