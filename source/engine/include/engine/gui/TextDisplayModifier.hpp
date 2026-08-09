#pragma once

namespace sf {class RenderTexture;}

namespace engine::gui
{

class TextDisplayModifier
{
public:
    virtual ~TextDisplayModifier() = default;
    virtual void render(sf::RenderTexture& renderTexture) = 0;
};

}  // namespace engine::gui
