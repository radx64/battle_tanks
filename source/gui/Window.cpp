#include "gui/Window.hpp"

#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Button.hpp"
#include "gui/Label.hpp"
#include "gui/Component.hpp"

constexpr auto MINIMUM_WINDOW_HEIGHT = 300.f;
constexpr auto MINIMUM_WINDOW_WIDTH = 300.f;
namespace gui
{

Window::Window()
: killed_{false}
, focused_{false}
, state_{State::Idle}
{   
    auto top_bar = std::make_unique<gui::TopBar>();

    top_bar->closeButtonAction([window = this](){window->close();});

    top_bar_handle_ = top_bar.get();
    top_bar_handle_->setSize({getSize().x, TOP_BAR_HEIGHT});
    Component::addChild(std::move(top_bar));

    auto window_panel = std::make_unique<gui::WindowPanel>();
    window_panel_handle_ = window_panel.get();
    Component::addChild(std::move(window_panel));

    auto bottom_bar = std::make_unique<gui::BottomBar>();
    bottom_bar->setSize(getSize());
    bottom_bar_handle_ = bottom_bar.get();
    Component::addChild(std::move(bottom_bar));
}

void Window::setTitle(const std::string_view& text)
{
    top_bar_handle_->setTitle(text);
}

bool Window::isInsideTopBar(const sf::Vector2f point)
{
    return top_bar_handle_->isInside(point);
}

bool Window::isInsideResizeThingy(const sf::Vector2f point)
{
    return bottom_bar_handle_->isInsideResizeThingy(point);
}

void Window::onRender(sf::RenderTexture& renderWindow)
{
    static_cast<void>(renderWindow);
}

bool Window::isState(const Window::State& state) const
{
    return state_ == state;
}

void Window::close()
{
    setVisibility(false);
    killed_ = true;
}

bool Window::isDead() const
{
    return killed_;
}

void Window::focus()
{
    focused_ = true;
    top_bar_handle_->onFocus();
    window_panel_handle_->onFocus();
    bottom_bar_handle_->onFocus();
}

void Window::defocus()
{
    focused_ = false;
    top_bar_handle_->onFocusLost();
    window_panel_handle_->onFocusLost();
    bottom_bar_handle_->onFocusLost();
    state_ = State::Idle;
}

bool Window::isFocused() const
{
    return focused_;
}

EventStatus Window::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};

    // If mouse clicked on top bar and was not yet dragging window
    if (isInsideTopBar(mousePosition) and isState(State::Idle))
    {
        state_ = State::Dragging; 
        disableChildrenEvents();
        dragging_offset_ = getPosition() -  mousePosition;
        return gui::EventStatus::Consumed;
    }

    if (isInsideResizeThingy(mousePosition) and isState(State::Idle))
    {
        state_ = State::Resizing;
        disableChildrenEvents();
        return gui::EventStatus::Consumed;
    }

    return gui::EventStatus::NotConsumed;
}

EventStatus Window::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    UNUSED(mouseButtonReleasedEvent);

    if (state_ != State::Idle) 
    {
        state_ = State::Idle;
        enableChildrenEvents();
        return gui::EventStatus::Consumed;
    }

    return gui::EventStatus::NotConsumed;
}

EventStatus Window::on(const event::MouseMoved& mouseMovedEvent)
{
    auto mousePosition = sf::Vector2f{mouseMovedEvent.position.x, mouseMovedEvent.position.y};

    if (isState(State::Dragging))
    {
        disableChildrenEvents();
        setPosition(mousePosition + dragging_offset_);
        return gui::EventStatus::Consumed;
    }

    if (isState(State::Resizing))
    {
        disableChildrenEvents();
        auto window_top_left_corner = getGlobalPosition();
        //auto old_window_size = getSize();
        auto new_window_size = mousePosition - window_top_left_corner + sf::Vector2f{RESIZE_THINGY_SIZE, RESIZE_THINGY_SIZE}/2.f;
        new_window_size.x = std::max(new_window_size.x, MINIMUM_WINDOW_WIDTH);
        new_window_size.y = std::max(new_window_size.y, MINIMUM_WINDOW_HEIGHT);
        setSize(new_window_size);

        // TODO: This is a hack as recalulating children size on resize is not working properlu
        // I need to investigate that
        //setPosition(getPosition());
        return gui::EventStatus::Consumed;
    }

    return gui::EventStatus::NotConsumed;
}

void Window::onPositionChange()
{
    top_bar_handle_->setPosition({0.f, 0.f});
    window_panel_handle_->setPosition({0.f, TOP_BAR_HEIGHT});
    bottom_bar_handle_->setPosition({0.f, getSize().y-RESIZE_THINGY_SIZE});
}

void Window::onSizeChange()
{
    auto size = getSize();
    top_bar_handle_->setSize({size.x, TOP_BAR_HEIGHT});
    window_panel_handle_->setSize({size.x, size.y - TOP_BAR_HEIGHT - RESIZE_THINGY_SIZE}); 
    bottom_bar_handle_->setSize({size.x, RESIZE_THINGY_SIZE});
    bottom_bar_handle_->setPosition({0.f, getSize().y-RESIZE_THINGY_SIZE});
}

}  // namespace gui