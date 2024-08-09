#pragma once

#include <cstdint>
#include <string_view>

#include <SFML/Graphics.hpp>

namespace gui
{

/*  This is a wrapper for sf::Text that alows partial text rendering
    in restricted bounding box
*/

class Text : public sf::Drawable
{
public:
    Text();

    void setText(const std::string_view& text);
    std::string getText() const;

    void setFont(const sf::Font& font);
    void setCharacterSize(uint32_t size);
    void setOutlineColor(const sf::Color& color);
    void setFillColor(const sf::Color& color);
    void setOutlineThickness(float thickness);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setOffset(const sf::Vector2f& offset);
    void setGlobalPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);

    sf::Vector2f getGlobalPosition() const;
    sf::Vector2f getSize() const;
    float getTextWidth() const;

protected:
    void updateTexture();
    sf::Vector2f offset_;
    sf::Vector2f global_position_;
    sf::Vector2f size_;
    sf::RenderTexture texture_;
    sf::Text text_;
    std::string buffer_;
};
    
}  // namespace gui