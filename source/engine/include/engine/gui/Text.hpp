#pragma once

#include <cstdint>
#include <string_view>

#include <SFML/Graphics.hpp>

#include "engine/gfx/Color.hpp"
#include "engine/Vector2.hpp"
#include "engine/Rect.hpp"
#include "engine/gfx/Drawable.hpp"
#include "engine/gfx/RenderTexture.hpp"
#include "engine/gfx/Sprite.hpp"

#include "engine/gui/style/Text.hpp"

namespace engine::gui {class TextDisplayModifier;}

namespace engine::gui
{

/*  This is a wrapper for sf::Text that alows partial text rendering
    in restricted bounding box
*/

class Text : public engine::gfx::Drawable
{
public:
    Text(const style::Text& style);

    void setText(const std::string_view& text);
    std::string getText() const;

    void setFont(const sf::Font& font);
    const sf::Font* getFont() const;

    void setCharacterSize(uint32_t size);
    uint32_t getCharacterSize() const;

    void setOutlineColor(const engine::gfx::Color& color);
    void setFillColor(const engine::gfx::Color& color);
    void setOutlineThickness(float thickness);
    
    void setOffset(const engine::Vec2f& offset);
    const engine::Vec2f& getOffset() const;
    void setGlobalPosition(const engine::Vec2f& position);
    void setSize(const engine::Vec2f& size);
    
    engine::Vec2f getGlobalPosition() const;
    engine::Vec2f getSize() const;
    engine::FloatRect getTextBounds() const;
    float getTextSingleLineHeight() const;
    
    void addModifier(TextDisplayModifier* modifier);
    void removeModifier(TextDisplayModifier* modifier);
    
    void invalidateTexture();
    void updateTexture();
    
protected:
    void updateSprite();
    engine::Vec2f offset_;
    engine::Vec2f globalPosition_;
    engine::Vec2f size_;
    engine::gfx::RenderTexture texture_;
    engine::gfx::Sprite sprite_;
    sf::Text text_;
    
    std::vector<TextDisplayModifier*> modifiers_;
    
    bool textureInvalidated_{false};
    
    const style::Text& style_;
    
private:
    void draw(engine::gfx::RenderTarget& target, const engine::gfx::RenderStates& states) const override;
};

}  // namespace engine::gui
