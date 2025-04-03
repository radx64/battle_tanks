#include "gui/ProgressBar.hpp"

#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"
#include "gui/TextureLibrary.hpp"

#include <string>

namespace
{

const auto BORDER_OFFSET = sf::Vector2f{10.f, 10.f};

gui::FramedSprite::LayoutConfig buildLayoutConfig(const sf::Vector2f& cornerSizes, const gui::FramedSprite::LayoutConfig::UVs& uvs)
{
    gui::FramedSprite::LayoutConfig layoutConfig{
        .cornerSizes = 
        {
            .topLeft        = {cornerSizes.x, cornerSizes.y},
            .bottomRight    = {cornerSizes.x, cornerSizes.y}
        },
        .uvs = uvs
    };

    return layoutConfig;
} 

gui::FramedSprite::LayoutConfig::UVs buildUVsForProgressBarTexture()
{
    return gui::FramedSprite::LayoutConfig::UVs
    {
        .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
        .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
        .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
        .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
    };
}

gui::FramedSprite::LayoutConfig buildLayoutConfigForProgressBarTexture()
{
    static auto layout = buildLayoutConfig({4.f, 4.f}, buildUVsForProgressBarTexture());
    return layout;
}

}  // namespace


namespace gui
{

 std::unique_ptr<ProgressBar> ProgressBar::create()
 {
    return std::unique_ptr<ProgressBar>{new ProgressBar{}};
 }

ProgressBar::ProgressBar()
: background_{buildLayoutConfigForProgressBarTexture()}
, normalTexture_{TextureLibrary::instance().get("progressbar_background")}
{
    auto style = BasicStyleSheetFactory::instance();
    //FIXME: I need to redesign style "subsystem" :)
    bar_.setFillColor(style.getWindowHeaderColor());
    bar_.setOutlineThickness(0.f);
    bar_.setPosition(getGlobalPosition());

    auto textPtr = gui::Label::create("");
    text_ = textPtr.get();
    text_->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
    addChild(std::move(textPtr));
    setTextLabel(progress_);

    background_.setTexture(normalTexture_);
    recalculateSize();
}

void ProgressBar::setRange(const float min, const float max)
{
    min_ = min;
    max_ = max;
}

void ProgressBar::setValue(const float value)
{
    value_ = std::clamp(value, min_, max_);
    progress_ = (value_ - min_) / (max_ - min_);

    setTextLabel(progress_);
    recalculateSize();
}

float ProgressBar::getValue() const
{
    return value_;
}

void ProgressBar::setTextLabel(const float value)
{
    char buffer[10];
    std::sprintf(buffer, "%.2f%%", value * 100.f);
    std::string text(buffer);

    text_->setText(text);
}

void ProgressBar::onPositionChange()
{
    background_.setPosition(getGlobalPosition());
    bar_.setPosition(getGlobalPosition() + BORDER_OFFSET);
}

void ProgressBar::onSizeChange()
{
    recalculateSize();
}

void ProgressBar::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    renderTexture.draw(bar_);
}

void ProgressBar::recalculateSize()
{
    auto componentSize = getSize();
    background_.setSize(componentSize);

    auto componentSizeWithOffset = componentSize - BORDER_OFFSET * 2.f;
    bar_.setSize(sf::Vector2f{componentSizeWithOffset.x * progress_, componentSizeWithOffset.y});
    
    text_->setSize(componentSize);    
}

}  // namespace
