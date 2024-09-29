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

    auto windowPanel = std::make_unique<gui::Panel>();
    windowPanel_ = windowPanel.get();
    Component::addChild(std::move(windowPanel));

    auto statusBar = std::make_unique<gui::window::StatusBar>();
    statusBar_ = statusBar.get();
    statusBar_->setSize(getSize());

    Component::addChild(std::move(statusBar));
}

void Window::addChild(std::unique_ptr<Component> component)
{
    windowPanel_->addChild(std::move(component));
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
    return statusBar_->isInsideResizeGadget(point);
}

void Window::onRender(sf::RenderTexture& renderTexture)
{
    UNUSED(renderTexture);
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
    statusBar_->activate();
}
void Window::deactivate()
{
    active_ = false;
    header_->deactivate();
    statusBar_->deactivate();
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
        draggingOffset_ = getPosition() -  mousePosition;
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
        setPosition(mousePosition + draggingOffset_);
        return gui::EventStatus::Consumed;
    }

    if (isInState(State::Resizing))
    {
        disableChildrenEvents();
        auto windowTopLeftCorner = getGlobalPosition();
        auto newWindowSize = mousePosition - windowTopLeftCorner 
            + sf::Vector2f{window::config::RESIZE_BOX_SIZE, window::config::RESIZE_BOX_SIZE}/2.f;
        newWindowSize.x = std::max(newWindowSize.x, MINIMUM_WINDOW_WIDTH);
        newWindowSize.y = std::max(newWindowSize.y, MINIMUM_WINDOW_HEIGHT);
        setSize(newWindowSize);
        return gui::EventStatus::Consumed;
    }

    return gui::EventStatus::NotConsumed;
}

void Window::onPositionChange()
{
    header_->setPosition({0.f,0.f});
    windowPanel_->setPosition({0.f, window::config::HEADER_HEIGHT});
    statusBar_->setPosition({0.f, getSize().y - window::config::RESIZE_BOX_SIZE});
}

void Window::onSizeChange()
{
    auto size = getSize();
    header_->setSize({size.x, window::config::HEADER_HEIGHT});
    windowPanel_->setSize({size.x, size.y - window::config::HEADER_HEIGHT - window::config::RESIZE_BOX_SIZE}); 
    statusBar_->setSize({size.x, window::config::RESIZE_BOX_SIZE});
    statusBar_->setPosition({0.f, getSize().y - window::config::RESIZE_BOX_SIZE});
}

}  // namespace gui
