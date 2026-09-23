#include "FramedSpriteRenderer.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include "engine/gui/FramedSprite.hpp"
#include "SfmlRenderTarget.hpp"

namespace engine::backends::sfml
{

void FramedSpriteRenderer::draw(sf::RenderTarget& target, const engine::gui::FramedSprite& sprite)
{
    SfmlRenderTarget sfmlTarget{target};
    sfmlTarget.draw(sprite);
}

}  // namespace engine::backends::sfml
