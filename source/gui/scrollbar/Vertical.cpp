#include "gui/scrollbar/Vertical.hpp"

#include <memory>

#include "gui/scrollbar/Base.hpp"

#include "gui/Button.hpp"
#include "gui/layout/Vertical.hpp"
#include "gui/slider/VerticalThick.hpp"

namespace gui::scrollbar
{

std::unique_ptr<Vertical> Vertical::create() 
{
    return std::unique_ptr<Vertical>(new Vertical{});
}

void Vertical::setThumbRatio(const float ratio)
{
    slider_ptr_->setThumbRatio(std::min(ratio, 1.0f));
}

Vertical::Vertical()
{
    auto layout = gui::layout::Vertical::create();
    layout->setPadding(0.f);

    auto slider = gui::slider::VerticalThick::create();
    auto upButton = gui::IconButton::create(gui::TextureLibrary::instance().get("arrow_up"));
    auto downButton = gui::IconButton::create(gui::TextureLibrary::instance().get("arrow_down"));

    slider->setRange(0.f, 1.f);
    slider->setStep(0.1f);
    slider->setValue(1.f);
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


} // namespace gui::scrollbar
