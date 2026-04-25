#pragma once

#include <memory>
#include <string_view>

#include <SFML/Graphics.hpp>

#include "gui/Text.hpp"
#include "gui/Widget.hpp"

namespace gui
{

class GroupBox : public Widget
{
public:
    static std::unique_ptr<GroupBox> create(const std::string_view& title);

    explicit GroupBox(const std::string_view& title);

    void addChild(std::unique_ptr<Widget> child) override;

    void setTitle(const std::string_view& title);
    std::string getTitle() const;

    void setPadding(float padding);
    float getPadding() const;
    void setOuterPadding(float padding);
    float getOuterPadding() const;

    void setBorderColor(const sf::Color& color);
    void setBorderThickness(float thickness);

    sf::FloatRect getContentBounds() const;

protected:
    void onRender(sf::RenderTexture& renderTexture) override;
    void onSizeChange() override;
    void onPositionChange() override;

private:
    void updateLayout();
    void updateChildrenLayout();
    void drawFrameSegment(sf::RenderTexture& renderTexture, const sf::FloatRect& rect) const;

    Text titleText_;
    sf::Color borderColor_;
    float borderThickness_;
    float outerPadding_;
    float padding_;
    float titlePadding_;
    float titleToFrameSpacing_;
    sf::FloatRect contentBounds_;
};

}  // namespace gui
