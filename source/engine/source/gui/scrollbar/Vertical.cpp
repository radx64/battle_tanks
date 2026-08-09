#include "engine/gui/scrollbar/Vertical.hpp"

#include <memory>

#include "engine/gui/scrollbar/Base.hpp"

#include "engine/gui/Button.hpp"
#include "engine/gui/layout/Vertical.hpp"
#include "engine/gui/slider/VerticalThick.hpp"

namespace engine::gui::scrollbar
{

std::unique_ptr<Vertical> Vertical::create() 
{
    return std::unique_ptr<Vertical>(new Vertical{});
}

void Vertical::setValue(float value)
{
    slider_ptr_->setValue(value);
}

float Vertical::getValue() const
{
    return slider_ptr_->getValue();
}

void Vertical::setStep(float step)
{
    slider_ptr_->setStep(step);
}

void Vertical::setThumbRatio(const float ratio)
{
    slider_ptr_->setThumbRatio(std::min(ratio, 1.0f));
}

Vertical::Vertical()
{
    auto layout = engine::gui::layout::Vertical::create();
    layout->setPadding(0.f);

    auto slider = engine::gui::slider::VerticalThick::create();
    auto upButton = engine::gui::IconButton::create(engine::gui::TextureLibrary::instance().get("arrow_up"));
    auto downButton = engine::gui::IconButton::create(engine::gui::TextureLibrary::instance().get("arrow_down"));

    slider->setValue(1.f);
    slider->setRange(0.f, 1.f);
    slider->setStep(0.01f);
    slider->onValueChange([this](const float value) {notifyChange(value);});

    upButton->onClick([sliderPtr = slider.get()]{sliderPtr->increase();});
    downButton->onClick([sliderPtr = slider.get()]{sliderPtr->decrease();});

    slider_ptr_ = slider.get();

    layout->addChild(std::move(upButton));
    layout->addChild(std::move(slider));
    layout->addChild(std::move(downButton));

    layout->setRowSize(0, layout::Constraint::Pixels(32.f));
    layout->setRowSize(2, layout::Constraint::Pixels(32.f));   

    layout_ptr_  = layout.get();

    addChild(std::move(layout));
}

void Vertical::onSizeChange()
{
    layout_ptr_->setSize(getSize());
}

void Vertical::onPositionChange()
{

}

void Vertical::onRender(sf::RenderTexture& renderTexture)
{
    (void)renderTexture;
}


} // namespace engine::gui::scrollbar
