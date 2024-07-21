#pragma once


#include <functional>

#include <SFML/Graphics.hpp>

#include "gui/Button.hpp"
#include "gui/Component.hpp"
#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"

constexpr auto TOP_BAR_HEIGHT = 30.f;
constexpr auto RESIZE_THINGY_SIZE = 20.f;

namespace gui { class Label; }
namespace gui
{

class WindowPanel : public Component
{
public:
    WindowPanel()
    {
        background_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
        background_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
        background_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());
    }

    void onSizeChange() override
    {
        auto panel_size = parent_->getSize();
        panel_size.y = panel_size.y - TOP_BAR_HEIGHT - RESIZE_THINGY_SIZE;

        auto background_size = panel_size;
        background_.setSize(background_size);
    }

    // TODO: simmilar as TODO for setSize method
    void onPositionChange() override
    {
        background_.setPosition(Component::getGlobalPosition());
    }

    void onRender(sf::RenderTexture& renderWindow) override
    {
        renderWindow.draw(background_);
    }

    void onFocus()
    {
        background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
    }

    void onFocusLost()
    {
        background_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
    }

protected:
    sf::RectangleShape background_;
    gui::StyleSheet style_;
};

class TopBar : public Component
{
public:
    TopBar()
    {
        top_bar_shape_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
        top_bar_shape_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());

        // TODO add layout component for button and title text
        auto close_button = std::make_unique<gui::Button>("X");
        close_button->setSize(sf::Vector2f(20.f, TOP_BAR_HEIGHT));
        close_button->onClick([this]()
        {
            if (closeButtonAction_) closeButtonAction_();
        });
        close_button_ptr_ = close_button.get();   
        addChild(std::move(close_button));

        auto title_text = std::make_unique<gui::Label>("");
        title_text->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        title_text_handle_ = title_text.get();
        title_text_handle_->setSize(getSize());
        addChild(std::move(title_text));
    }

    void closeButtonAction(std::function<void()> closeButtonAction)
    {
        closeButtonAction_ = closeButtonAction;
    }

    void setTitle(const std::string_view& text)
    {
        title_text_handle_->setText(text.data());
        // after setText called Label has default bounds
        // so I need to resize it to fill the bar
        title_text_handle_->setSize(getSize());
    }

    void onRender(sf::RenderTexture& renderWindow) override
    {
        renderWindow.draw(top_bar_shape_);
    }

    // FIXME component size should be same as renderable
    // DAMN i have no idea what I've meant by this comment
    void onSizeChange() override
    {
        auto top_bar_size = getSize();
        top_bar_size.y = TOP_BAR_HEIGHT;
        top_bar_shape_.setSize(top_bar_size);
        title_text_handle_->setSize(top_bar_size);
        close_button_ptr_->setPosition(sf::Vector2f{top_bar_size.x - 20.f, 0.f});
    }

    void onPositionChange() override
    {
        top_bar_shape_.setPosition(Component::getGlobalPosition());
        
        // TODO: placement of text label should be composed by some layout
        sf::Vector2f tile_text_position {0.0f ,0.0f}; 
        title_text_handle_->setPosition(tile_text_position);
    }

    void onFocus()
    {
        top_bar_shape_.setFillColor(BasicStyleSheetFactory::instance().getTopBarWindowColor());
    }

    void onFocusLost()
    {
        top_bar_shape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveTopBarWindowColor());
    }

    bool isInside(const sf::Vector2f point)
    {
        return top_bar_shape_.getGlobalBounds().contains(point);
    }

protected:
    gui::Label* title_text_handle_;
    sf::RectangleShape top_bar_shape_;
    gui::Button* close_button_ptr_;
    std::function<void()> closeButtonAction_;
};

class BottomBar : public Component
{
public:
    BottomBar()
    {
        background_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
        background_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
        background_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());

        resize_thingy_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
        resize_thingy_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());
        resize_thingy_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());

        resize_thingy_.setSize(sf::Vector2f{RESIZE_THINGY_SIZE, RESIZE_THINGY_SIZE});
    }

    void onRender(sf::RenderTexture& renderWindow) override
    {
        renderWindow.draw(background_);
        renderWindow.draw(resize_thingy_);
    }

    // FIXME component size should be same as renderable
    void onSizeChange() override
    {
        auto bottom_bar_size = getSize();
        bottom_bar_size.y = RESIZE_THINGY_SIZE;
        background_.setSize(bottom_bar_size);

        resize_thingy_.setPosition(
            Component::getGlobalPosition() 
            + Component::getSize() 
            - sf::Vector2f{RESIZE_THINGY_SIZE, RESIZE_THINGY_SIZE});
    }

    void onPositionChange() override
    {
        background_.setPosition(getGlobalPosition());

        resize_thingy_.setPosition(
            Component::getGlobalPosition() 
            + Component::getSize() 
            - sf::Vector2f{RESIZE_THINGY_SIZE, RESIZE_THINGY_SIZE});
    }

    void onFocus()
    {
        background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
    }

    void onFocusLost()
    {
        background_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
    }

    bool isInsideResizeThingy(const sf::Vector2f point)
    {
        return resize_thingy_.getGlobalBounds().contains(point);
    }

protected:
    sf::RectangleShape resize_thingy_;
    sf::RectangleShape background_;
};


// Class for main application window layer
// It should always be as a backlayer
// Can be used to place GUI components
// on screen
// Can't be moved
// It's size is application window size  
class MainWindow : public Component
{
public:
    void onRender(sf::RenderTexture& renderWindow) override
    {
        UNUSED(renderWindow);
    }
};

class Window : public Component
{
public:
    Window();

    void setTitle(const std::string_view& text);
    void onRender(sf::RenderTexture& renderWindow) override;
    void onPositionChange() override;
    void onSizeChange() override;
    void close();
    bool isDead() const;
    void focus();
    void defocus();
    bool isFocused() const;

    bool isIdle() const
    {
        return isState(Window::State::Idle);
    }

    void addChild(std::unique_ptr<Component> child) override;

protected:
    enum class State
    {
        Idle,
        Dragging,
        Resizing
    };
    bool isState(const Window::State& state) const;
    bool isInsideTopBar(const sf::Vector2f point);
    bool isInsideResizeThingy(const sf::Vector2f point);

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;

    bool killed_;
    bool focused_;
    State state_;
    sf::Vector2f dragging_offset_;

    //TODO: weak_ptrs maybe?
    gui::TopBar* top_bar_handle_;
    gui::WindowPanel* window_panel_handle_;
    gui::BottomBar* bottom_bar_handle_;
};

}  // namespace gui

