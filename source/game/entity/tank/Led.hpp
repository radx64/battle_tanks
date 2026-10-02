#pragma once


#include "engine/Vector2.hpp"
#include "engine/gfx/Color.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/Sprite.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity 
{

class Led
{
public:
    Led(float x, float y, engine::gfx::Texture& texture);
    void draw(engine::gfx::RenderTarget& render_window);
    void update(float time_step);
    void setRotation(float rotation);
    
    float x_;
    float y_;

    void setColor(const engine::gfx::Color& color);
    void setBlinkingRate(float rate);  // TODO use types from clock to have clearly s, ms etc...
    void set(bool state);
    
protected:
    const engine::Vector2f offset_{10.0f, 15.0f};  // Offset from tank center
    engine::gfx::Color color_{engine::gfx::Color::Green};
    bool isOn_{false};
    float rate_{}; 
    float timeToChangeState_;
    float rotation_{0.0f};
    engine::gfx::Sprite sprite_;
};

}  // namespace game::entity
