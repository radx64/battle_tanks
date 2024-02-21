#ifndef SANDBOX_APPLICATION_HPP_
#define SANDBOX_APPLICATION_HPP_

#include "engine/Application.hpp"
#include "MouseController.hpp"

namespace sandbox
{
class Application : public engine::Application
{
public:
    Application();

protected:
    void onInit() override;
    void onClose() override;
    void onEvent(const sf::Event& event) override;
    void onUpdate(float timeStep) override;
    void onRender() override; 

    MouseController mouse_controller_;   
};

}  // namespace sandbox

#endif  // SANDBOX_APPLICATION_HPP_

