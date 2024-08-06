#include "gui/ProgressBar.hpp"

#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"

#include <string>

namespace gui
{

ProgressBar::ProgressBar()
{
    auto style = BasicStyleSheetFactory::instance();    
    background_.setFillColor(style.getWindowColor());
    background_.setOutlineColor(style.getOutlineColor());
    background_.setOutlineThickness(style.getOutlineThickness()); 
    background_.setPosition(getGlobalPosition());
    background_.setSize(Component::getSize());

    //FIXME: I need to redesign style "subsystem" :)
    bar_.setFillColor(style.getWindowHeaderColor());
    bar_.setOutlineColor(style.getOutlineColor());
    bar_.setOutlineThickness(style.getOutlineThickness()); 
    bar_.setPosition(getGlobalPosition());

    auto text_ptr = std::make_unique <gui::Label>("");
    text_ = text_ptr.get();
    text_->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
    addChild(std::move(text_ptr));
    setTextLabel(progress_);
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

    auto componentSize = getSize();
    bar_.setSize({componentSize.x * progress_, componentSize.y});
    bar_.setPosition(getGlobalPosition());
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
    text_->setSize(getSize());
}

void ProgressBar::onPositionChange()
{
    bar_.setPosition(getGlobalPosition());
}

void ProgressBar::onSizeChange()
{
    auto componentSize = getSize();
    bar_.setSize({componentSize.x * progress_, componentSize.y});
    text_->setSize(getSize());
}

void ProgressBar::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    renderTexture.draw(bar_);
}

}  // namespace