#include "gui/Window.hpp"

#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Button.hpp"
#include "gui/Component.hpp"
#include "gui/Label.hpp"
#include "gui/Panel.hpp"

constexpr auto MINIMUM_WINDOW_HEIGHT = 300.f;
constexpr auto MINIMUM_WINDOW_WIDTH = 300.f;
namespace gui
{

Window::Window()
: killed_{false}
, active_{false}
, state_{State::Idle}
{   
    auto header = std::make_unique<window::Header>();
    header_ = header.get();
    header_->closeButtonAction([window = this](){window->close();});
    header_->setSize({getSize().x, window::config::HEADER_HEIGHT});
    Component::addChild(std::move(header));

    auto window_panel = std::make_unique<gui::Panel>();
    window_panel_ = window_panel.get();
    Component::addChild(std::move(window_panel));

    auto status_bar = std::make_unique<gui::window::StatusBar>();
    status_bar_ = status_bar.get();
    status_bar_->setSize(getSize());

    Component::addChild(std::move(status_bar));
}

void Window::addChild(std::unique_ptr<Component> component)
{
    window_panel_->addChild(std::move(component));
}

void Window::setTitle(const std::string_view& text)
{
    header_->setTitle(text);
}

bool Window::isInsideHeader(const sf::Vector2f point)
{
    return header_->isInside(point);
}

bool Window::isInsideResizeGadget(const sf::Vector2f point)
{
    return status_bar_->isInsideResizeGadget(point);
}

void Window::onRender(sf::RenderTexture& renderTexture)
{
    static_cast<void>(renderTexture);
}

bool Window::isInState(const Window::State& state) const
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

void Window::activate()
{
    active_ = true;
    header_->activate();
    status_bar_->activate();
}
void Window::deactivate()
{
    active_ = false;
    header_->deactivate();
    status_bar_->deactivate();
}

bool Window::isActive() const
{
    return active_;
}

bool Window::isIdle() const
{
    return isInState(Window::State::Idle);
}

EventStatus Window::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};

    // If mouse clicked on top bar and was not yet dragging window
    if (isInsideHeader(mousePosition) and isInState(State::Idle))
    {
        state_ = State::Dragging; 
        disableChildrenEvents();
        dragging_offset_ = getPosition() -  mousePosition;
        return gui::EventStatus::Consumed;
    }

    if (isInsideResizeGadget(mousePosition) and isInState(State::Idle))
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

    if (isInState(State::Dragging))
    {
        disableChildrenEvents();
        setPosition(mousePosition + dragging_offset_);
        return gui::EventStatus::Consumed;
    }

    if (isInState(State::Resizing))
    {
        disableChildrenEvents();
        auto window_top_left_corner = getGlobalPosition();
        //auto old_window_size = getSize();
        auto new_window_size = mousePosition - window_top_left_corner 
            + sf::Vector2f{window::config::RESIZE_BOX_SIZE, window::config::RESIZE_BOX_SIZE}/2.f;
        new_window_size.x = std::max(new_window_size.x, MINIMUM_WINDOW_WIDTH);
        new_window_size.y = std::max(new_window_size.y, MINIMUM_WINDOW_HEIGHT);
        setSize(new_window_size);
        return gui::EventStatus::Consumed;
    }

    return gui::EventStatus::NotConsumed;
}

void Window::onPositionChange()
{
    header_->setPosition({0.f,0.f});
    window_panel_->setPosition({0.f, window::config::HEADER_HEIGHT});
    status_bar_->setPosition({0.f, getSize().y - window::config::RESIZE_BOX_SIZE});
}

void Window::onSizeChange()
{
    auto size = getSize();
    header_->setSize({size.x, window::config::HEADER_HEIGHT});
    window_panel_->setSize({size.x, size.y - window::config::HEADER_HEIGHT - window::config::RESIZE_BOX_SIZE}); 
    status_bar_->setSize({size.x, window::config::RESIZE_BOX_SIZE});
    status_bar_->setPosition({0.f, getSize().y - window::config::RESIZE_BOX_SIZE});
}

}  // namespace gui