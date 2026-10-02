#pragma once

#include <vector>

#include "engine/Application.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gfx/Texture.hpp"
#include "MouseController.hpp"
#include "KeyboardController.hpp"

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
    void onEvent(engine::events::Event& event) override;
    void onUpdate(float time_step) override;
    void onRender() override; 

    void drawBody(b2BodyId bodyId);

    MouseController mouseController_;
    KeyboardController keyboardController_;


    b2WorldId worldId_;
    std::vector<b2BodyId> bodies_;

    engine::gfx::Texture boxTexture_;
    engine::gfx::Texture circleTexture_;

    engine::gfx::Sprite boxSprite_;
    engine::gfx::Sprite circleSprite_; 
    bool debugDraw_ = false;
};

}  // namespace sandbox
