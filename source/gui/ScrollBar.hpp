#pragma once

#include <memory>

#include "gui/Component.hpp"

#include "gui/Button.hpp"
#include "gui/slider/VerticalThick.hpp"
#include "gui/TextureLibrary.hpp"

#include "gui/layout/Vertical.hpp"

namespace gui
{
class VerticalScrollBar : public gui::Component
{
public:
    static std::unique_ptr<gui::VerticalScrollBar> create() 
    {
        return std::unique_ptr<VerticalScrollBar>(new VerticalScrollBar{});
    }

protected:
    VerticalScrollBar()
    : layout_ptr{nullptr}
    {
        auto layout = gui::layout::Vertical::create();
        layout->setPadding(0.f);

        auto slider = gui::slider::VerticalThick::create();
        auto upButton = gui::IconButton::create(gui::TextureLibrary::instance().get("arrow_up"));
        auto downButton = gui::IconButton::create(gui::TextureLibrary::instance().get("arrow_down"));

        slider->setRange(0.f, 1.f);
        slider->setStep(0.1f);
        slider->setValue(1.f);

        upButton->onClick([sliderPtr = slider.get()]{sliderPtr->increase();});
        downButton->onClick([sliderPtr = slider.get()]{sliderPtr->decrease();});

        layout->addChild(std::move(upButton));
        layout->addChild(std::move(slider));
        layout->addChild(std::move(downButton));

        layout_ptr  = layout.get();

        addChild(std::move(layout));
    }

    void onSizeChange() override
    {
        layout_ptr->setSize(getSize());
    }
    void onPositionChange() override
    {

    }

    void onRender(sf::RenderTexture& renderTexture) override
    {
        (void)renderTexture;
    }

    Component* layout_ptr;
};




}
