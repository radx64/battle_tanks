#include "gui/Window.hpp"

#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Button.hpp"
#include "gui/Label.hpp"
#include "gui/Component.hpp"

constexpr auto TOP_BAR_WIDTH = 20.f;
constexpr auto RESIZE_THINGY_SIZE = 20.f;
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
    background_.setFillColor(style_.getInactiveWindowColor());
    background_.setOutlineColor(style_.getOutlineColor());
    background_.setOutlineThickness(style_.getOutlineThickness());

    auto close_button = std::make_unique<gui::Button>("X");
    close_button->setPosition(sf::Vector2f(0.f, 0.f), gui::Alignment::LEFT);
    close_button->setSize(sf::Vector2f(20.f, TOP_BAR_WIDTH));
    close_button->onClick([window = this]()
    {
        //Prevent clicking close button when resizing or dragging
        if (window->isState(State::Idle))
        {
            window->close();
        }
    });
    this->addChild(std::move(close_button));

    // TODO: Consider creating top bar as a some decorator component 
    // and close button as a child of it (with some extra layout mechanism)
    top_bar_.setOutlineColor(style_.getOutlineColor());
    top_bar_.setOutlineThickness(style_.getOutlineThickness());

    resize_thingy_.setOutlineColor(style_.getOutlineColor());
    resize_thingy_.setOutlineThickness(style_.getOutlineThickness());
    resize_thingy_.setFillColor(style_.getInactiveWindowColor());

    // TODO: Title window text could be a child of a top bar (see todo above)
    auto title_text = std::make_unique<gui::Label>("");
    title_text_handle_ = title_text.get();
    this->addChild(std::move(title_text));

    resize_thingy_.setSize(sf::Vector2f{RESIZE_THINGY_SIZE, RESIZE_THINGY_SIZE});
}

void Window::setSize(const sf::Vector2f& size)
{
    auto top_bar_size = size;
    top_bar_size.y = TOP_BAR_WIDTH;

    auto background_size = size;
    background_size.y = size.y - TOP_BAR_WIDTH;

    top_bar_.setSize(top_bar_size);
    background_.setSize(background_size);
    Component::setSize(size);
    updateResizeThingy();
}

void Window::setTitle(const std::string_view& text)
{
    title_text_handle_->setText(text.data());
}

void Window::updateResizeThingy()
{
    // global position should not be used if those rendered elements like top bar and resize thingy
    // was a proper child of window
    // this is yet another FIXME :)
    const auto window_size = getSize();
    const auto window_position = getGlobalPosition();
    sf::Vector2f resize_thingy_position {window_position.x + window_size.x  - RESIZE_THINGY_SIZE, 
        window_position.y + window_size.y - RESIZE_THINGY_SIZE};
    resize_thingy_.setPosition(resize_thingy_position);
}


void Window::setPosition(const sf::Vector2f& position, const Alignment alignment)
{
    Component::setPosition(position, alignment);
    top_bar_.setPosition(Component::getGlobalPosition());
    auto background_position = Component::getGlobalPosition();
    background_position.y += TOP_BAR_WIDTH;
    background_.setPosition(background_position);

    const auto window_size = getSize();

    sf::Vector2f tile_text_position {window_size.x/2.f, TOP_BAR_WIDTH / 2.f};
    title_text_handle_->setPosition(tile_text_position, gui::Alignment::CENTERED);
    updateResizeThingy();
}

bool Window::isInside(const sf::Vector2f point)
{
    return bounds_.contains(point);
}

bool Window::isInsideTopBar(const sf::Vector2f point)
{
    return top_bar_.getGlobalBounds().contains(point);
}

bool Window::isInsideResizeThingy(const sf::Vector2f point)
{
    return resize_thingy_.getGlobalBounds().contains(point);
}

void Window::onRender(sf::RenderWindow& renderWindow)
{
    renderWindow.draw(background_);
    renderWindow.draw(top_bar_);
    renderWindow.draw(resize_thingy_);
}

bool Window::isState(const Window::State& state)
{
    return state_ == state;
}

bool Window::onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    // If mouse clicked on top bar and was not yet dragging window
    if (isInsideTopBar(mousePosition) and (not isState(State::Dragging)) and isLeftClicked)
    {
        state_ = State::Dragging; 
        dragging_offset_ = getPosition() -  mousePosition;
    }

    if (isInsideResizeThingy(mousePosition) and (not isState(State::Resizing)) and isLeftClicked)
    {
        state_ = State::Resizing;
    }

    // If mouse was released when dragging or resizing window
    if (not isLeftClicked and (isState(State::Dragging) or isState(State::Resizing)))
    {
        state_ = State::Idle;
    } 

    if (isState(State::Dragging))
    {
        setPosition(mousePosition + dragging_offset_, alignment_);
        return EVENT_PROCESSED;
    }

    if (isState(State::Resizing))
    {
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
    background_.setFillColor(style_.getWindowColor());
    top_bar_.setFillColor(style_.getTopBarWindowColor());
}

void Window::defocus()
{
    focused_ = false;
    background_.setFillColor(style_.getInactiveWindowColor());
    top_bar_.setFillColor(style_.getInactiveTopBarWindowColor());
    state_ = State::Idle;
}

bool Window::isFocused() const
{
    return focused_;
}

}  // namespace gui