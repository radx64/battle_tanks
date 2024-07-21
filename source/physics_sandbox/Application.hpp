#pragma once


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



