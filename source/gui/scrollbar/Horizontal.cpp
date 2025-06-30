#include "gui/scrollbar/Horizontal.hpp"

#include "gui/Button.hpp"
#include "gui/layout/Horizontal.hpp"
#include "gui/slider/HorizontalThick.hpp"

namespace gui::scrollbar
{

std::unique_ptr<Horizontal> Horizontal::create() 
{
    return std::unique_ptr<Horizontal>(new Horizontal{});
}

void Horizontal::setThumbRatio(const float ratio)
{
    slider_ptr_->setThumbRatio(std::min(ratio, 1.0f));
}

Horizontal::Horizontal()
{
    auto layout = gui::layout::Horizontal::create();
    layout->setPadding(0.f); 

    auto slider = gui::slider::HorizontalThick::create();
    auto leftButton = gui::IconButton::create(gui::TextureLibrary::instance().get("arrow_left"));
    auto rightButton = gui::IconButton::create(gui::TextureLibrary::instance().get("arrow_right"));

    slider->setRange(0.f, 1.f);
    slider->setStep(0.1f);
    slider->setValue(0.f);
    slider->onValueChange([this](const float value) {notifyChange(value);});

    leftButton->onClick([sliderPtr = slider.get()]{sliderPtr->decrease();});
    rightButton->onClick([sliderPtr = slider.get()]{sliderPtr->increase();});

    slider_ptr_ = slider.get();

    layout->addChild(std::move(leftButton));
    layout->addChild(std::move(slider));
    layout->addChild(std::move(rightButton));

    layout->setColumnSize(0, layout::Constraint::Pixels(32.f));
    layout->setColumnSize(2, layout::Constraint::Pixels(32.f));   

    layout_ptr_ = layout.get();

    addChild(std::move(layout));
}

void Horizontal::onSizeChange()
{
    layout_ptr_->setSize(getSize());
}

void Horizontal::onPositionChange()
{

}

void Horizontal::onRender(sf::RenderTexture& renderTexture)
{
    (void)renderTexture;
}

} // namespace gui::scrollbar
