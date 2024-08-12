#pragma once

#include <cstdint>
#include <string_view>

#include <SFML/Graphics.hpp>

namespace gui {class TextDisplayModifier;}

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
    const sf::Font* getFont() const;

    void setCharacterSize(uint32_t size);
    uint32_t getCharacterSize() const;

    void setOutlineColor(const sf::Color& color);
    void setFillColor(const sf::Color& color);
    void setOutlineThickness(float thickness);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setOffset(const sf::Vector2f& offset);
    const sf::Vector2f& getOffset() const;
    void setGlobalPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);

    sf::Vector2f getGlobalPosition() const;
    sf::Vector2f getSize() const;
    float getTextWidth() const;
    float getTextHeight() const;

    void addModifier(TextDisplayModifier* modifier);
    void removeModifier(TextDisplayModifier* modifier);
    
    void updateTexture();

protected:
    void updateSprite();
    sf::Vector2f offset_;
    sf::Vector2f global_position_;
    sf::Vector2f size_;
    sf::RenderTexture texture_;
    sf::Sprite sprite_;
    sf::Text text_;

    std::vector<TextDisplayModifier*> modifiers_;
};
    
}  // namespace gui
