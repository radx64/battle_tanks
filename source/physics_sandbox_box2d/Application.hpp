#pragma once

#include "engine/Application.hpp"
#include "MouseController.hpp"
#include "KeyboardController.hpp"

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

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

    void drawBody(b2BodyId bodyId);

    MouseController mouseController_;
    KeyboardController keyboardController_;


    b2WorldId worldId_;
    std::vector<b2BodyId> bodies_;

    sf::Texture boxTexture_;
    sf::Texture circleTexture_;

    sf::Sprite boxSprite_;
    sf::Sprite circleSprite_; 
    bool debugDraw_ = false;
};

}  // namespace sandbox
