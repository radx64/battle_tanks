#pragma once

#include <memory>

#include "gui/Component.hpp"

#include "gui/Button.hpp"
#include "gui/slider/HorizontalThick.hpp"
#include "gui/slider/VerticalThick.hpp"
#include "gui/TextureLibrary.hpp"

#include "gui/layout/Vertical.hpp"

namespace gui
{

class ScrollBarBase : public gui::Component
{
public:
    void onValueChange(std::function<void(float)> onChangeCallback)
    {
        onChangeCallback_ = onChangeCallback;
    }

protected:
    ScrollBarBase()
    : layout_ptr_{nullptr}
    { }
    
    void notifyChange(const float value)
    {
        if (onChangeCallback_) onChangeCallback_(value);
    }

    Component* layout_ptr_;
    std::function<void(float)> onChangeCallback_;
};

class VerticalScrollBar : public ScrollBarBase
{
public:
    static std::unique_ptr<gui::VerticalScrollBar> create() 
    {
        return std::unique_ptr<VerticalScrollBar>(new VerticalScrollBar{});
    }

    void setThumbRatio(const float ratio)
    {
        slider_ptr_->setThumbRatio(std::min(ratio, 1.0f));
    }

protected:
    VerticalScrollBar()
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

    void onSizeChange() override
    {
        layout_ptr_->setSize(getSize());
    }
    void onPositionChange() override
    {

    }

    void onRender(sf::RenderTexture& renderTexture) override
    {
        (void)renderTexture;
    }

    gui::slider::VerticalThick* slider_ptr_;
};

class HorizontalScrollBar : public ScrollBarBase
{
public:
    static std::unique_ptr<gui::HorizontalScrollBar> create() 
    {
        return std::unique_ptr<HorizontalScrollBar>(new HorizontalScrollBar{});
    }

    void setThumbRatio(const float ratio)
    {
        slider_ptr_->setThumbRatio(std::min(ratio, 1.0f));
    }

protected:
    HorizontalScrollBar()
    {
        auto layout = gui::layout::Horizontal::create();
        layout->setPadding(0.f); 

        auto slider = gui::slider::HorizontalThick::create();
        auto leftButton = gui::IconButton::create(gui::TextureLibrary::instance().get("arrow_left"));
        auto rightButton = gui::IconButton::create(gui::TextureLibrary::instance().get("arrow_right"));

        slider->setRange(0.f, 1.f);
        slider->setStep(0.1f);
        slider->setValue(1.f);
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

    void onSizeChange() override
    {
        layout_ptr_->setSize(getSize());
    }

    void onPositionChange() override
    {

    }

    void onRender(sf::RenderTexture& renderTexture) override
    {
        (void)renderTexture;
    }

    gui::slider::HorizontalThick* slider_ptr_;

    float ratio_;
};

}
