#pragma once

namespace sf
{
class RenderTarget;
}

namespace engine::gui
{
class FramedSprite;
}

namespace engine::backends::sfml
{

class FramedSpriteRenderer
{
public:
    static void draw(sf::RenderTarget& target, const engine::gui::FramedSprite& sprite);
};

}  // namespace engine::backends::sfml
