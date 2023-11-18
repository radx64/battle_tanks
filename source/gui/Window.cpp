#include "gui/Window.hpp"

#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Button.hpp"
#include "gui/Label.hpp"
#include "gui/Component.hpp"

constexpr auto MINIMUM_WINDOW_HEIGHT = 300.f;
constexpr auto MINIMUM_WINDOW_WIDTH = 300.f;

constexpr bool EVENT_PROCESSED = true;
constexpr bool EVENT_NOT_PROCESSED = false;

namespace gui
{

Window::Window()
: killed_{false}
, focused_{false}
, state_{State::Idle}
, style_(BasicStyleSheetFactory::create())
{   
    auto top_bar = std::make_unique<gui::TopBar>();

    top_bar->closeButtonAction([window = this]()
    {
        // Prevent clicking close button when resizing or dragging
        // FIXME: This need to be fixed in some better way 
        // as if any button is place inside window
        // It sometimes gets clicked

        if (window->isState(State::Idle))
        {
            window->close();
        }
    });

    top_bar_handle_ = top_bar.get();
    Component::addChild(std::move(top_bar));

    auto window_panel = std::make_unique<gui::WindowPanel>();
    window_panel_handle_ = window_panel.get();
    Component::addChild(std::move(window_panel));

    auto bottom_bar = std::make_unique<gui::BottomBar>();
    bottom_bar_handle_ = bottom_bar.get();
    Component::addChild(std::move(bottom_bar));
}

void Window::setSize(const sf::Vector2f& size)
{
    Component::setSize(size);
}

void Window::setTitle(const std::string_view& text)
{
    top_bar_handle_->setTitle(text);
}

bool Window::isInside(const sf::Vector2f point)
{
    return bounds_.contains(point);
}

bool Window::isInsideTopBar(const sf::Vector2f point)
{
    return top_bar_handle_->isInside(point);
}

bool Window::isInsideResizeThingy(const sf::Vector2f point)
{
    return bottom_bar_handle_->isInsideResizeThingy(point);
}

void Window::onRender(sf::RenderWindow& renderWindow)
{
    static_cast<void>(renderWindow);
}

bool Window::isState(const Window::State& state)
{
    return state_ == state;
}

bool Window::onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    // If mouse clicked on top bar and was not yet dragging window
    if (isInsideTopBar(mousePosition) and (isState(State::Idle)) and isLeftClicked)
    {
        state_ = State::Dragging; 
        dragging_offset_ = getPosition() -  mousePosition;
    }

    if (isInsideResizeThingy(mousePosition) and (isState(State::Idle)) and isLeftClicked)
    {
        state_ = State::Resizing;
    }

    // If mouse was released when dragging or resizing window
    if (not isLeftClicked and (isState(State::Dragging) or isState(State::Resizing)))
    {
        enableChildrenEvents();
        state_ = State::Idle;
    } 

    if (isState(State::Dragging))
    {
        disableChildrenEvents();
        setPosition(mousePosition + dragging_offset_, alignment_);
        return EVENT_PROCESSED;
    }

    if (isState(State::Resizing))
    {
        disableChildrenEvents();
        auto window_top_left_corner = getGlobalPosition();
        auto old_window_size = getSize();
        auto new_window_size = mousePosition - window_top_left_corner + sf::Vector2f{RESIZE_THINGY_SIZE, RESIZE_THINGY_SIZE}/2.f;
        new_window_size.x = std::max(new_window_size.x, MINIMUM_WINDOW_WIDTH);
        new_window_size.y = std::max(new_window_size.y, MINIMUM_WINDOW_HEIGHT);
        setSize(new_window_size);

        // This is needed to recalculate position of component 
        // (especially when alignment::Center is used as it changes top left global position)

        // TODO: reconsider if this alignment should be on window level or only widgets
        // Maybe window should not derive from component?
        // And component should be used only for widgets
        switch(alignment_)
        {
            case Alignment::LEFT: break;
            case Alignment::CENTERED: 
            {
                setPosition(getPosition() + (new_window_size - old_window_size) / 2.f, alignment_);
                break;
            }

            case Alignment::RIGHT:
            {
                auto window_offset = (new_window_size - old_window_size);
                setPosition(getPosition() + sf::Vector2f{window_offset.x, 0.f}, alignment_);
                break;
            }
        }

        // If currently resizing window process the mouse event
        return EVENT_PROCESSED;
        
    }

    return EVENT_NOT_PROCESSED;
}

void Window::close()
{
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

}  // namespace gui