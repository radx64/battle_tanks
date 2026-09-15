#include "engine/Window.hpp"

#include <SFML/Window.hpp>

#include "engine/events/ApplicationEvent.hpp"
#include "engine/events/KeyEvent.hpp"
#include "engine/events/MouseEvent.hpp"

#include "fromSf.hpp"

constexpr unsigned int ANTI_ALIASING_LEVEL = 4;

namespace engine
{

class Window::Impl
{
public:
    Impl(const WindowProperties& properties)
    : window_(sf::VideoMode(properties.size.x, properties.size.y, 32), properties.title,
        sf::Style::Default, sf::ContextSettings(0, 0, ANTI_ALIASING_LEVEL))
    {
        window_.setKeyRepeatEnabled(false);
        window_.setPosition(sf::Vector2i{0, 0});
    }

    sf::RenderWindow window_;
};

Window::Window(const WindowProperties& properties)
: impl_(std::make_unique<Impl>(properties))
{}

Window::~Window() = default;

void Window::setTitle(const std::string& title)
{
    impl_->window_.setTitle(title);
}

void Window::setSize(const engine::Vector2i& size)
{
    impl_->window_.setSize(sf::Vector2u(size.x, size.y));
}

void Window::setPosition(const engine::Vector2i& position)
{
    impl_->window_.setPosition(sf::Vector2i(position.x, position.y));
}

void Window::setKeyRepeatEnabled(bool enabled)
{
    impl_->window_.setKeyRepeatEnabled(enabled);
}

void Window::close()
{
    impl_->window_.close();
}

void Window::pollEvents()
{
    if (!event_callback_) return;

    sf::Event sfml_event;

    while(impl_->window_.pollEvent(sfml_event))
    {
        switch (sfml_event.type)
        {
            case sf::Event::Closed: 
            {
                events::WindowClosedEvent event{};
                event_callback_(event); 
                break;
            }
            case sf::Event::KeyPressed: 
            {
                events::KeyModifiers modifiers = 
                {
                    .alt = sfml_event.key.alt,
                    .shift = sfml_event.key.shift,
                    .control = sfml_event.key.control,
                    .system = sfml_event.key.system
                };

                events::KeyPressed event{static_cast<core::KeyCode>(sfml_event.key.code), modifiers};
                event_callback_(event);
                break;
            }
            case sf::Event::KeyReleased: 
            {
                events::KeyModifiers modifiers = 
                {
                    .alt = sfml_event.key.alt,
                    .shift = sfml_event.key.shift,
                    .control = sfml_event.key.control,
                    .system = sfml_event.key.system
                };

                events::KeyReleased event{static_cast<core::KeyCode>(sfml_event.key.code), modifiers};
                event_callback_(event);
                break;
            }

            case sf::Event::TextEntered:
            {
                events::TextEntered event{sfml_event.text.unicode};
                event_callback_(event);
                break;
            }

            case sf::Event::MouseMoved:
            {
                events::MouseMoved event{engine::Vec2f{static_cast<float>(sfml_event.mouseMove.x), static_cast<float>(sfml_event.mouseMove.y)}};
                event_callback_(event);
                break;
            }

            case sf::Event::MouseWheelScrolled:
            {
                const engine::Vec2f position {static_cast<float>(sfml_event.mouseWheelScroll.x), static_cast<float>(sfml_event.mouseWheelScroll.y)};
                const float offset = sfml_event.mouseWheelScroll.delta;
                
                const engine::core::MouseWheel wheel = backends::fromSf(sfml_event.mouseWheelScroll.wheel);
                events::MouseScrolled event{position, wheel, offset};
                event_callback_(event);
                break;

            }

            case sf::Event::MouseButtonPressed:
            {
                const engine::Vec2f position {static_cast<float>(sfml_event.mouseButton.x), static_cast<float>(sfml_event.mouseButton.y)};
                const engine::core::MouseButton button = backends::fromSf(sfml_event.mouseButton.button);
                events::MouseButtonPressed event{position, button};
                event_callback_(event);
                break;
            }

            case sf::Event::MouseButtonReleased:
            {
                const engine::Vec2f position {static_cast<float>(sfml_event.mouseButton.x), static_cast<float>(sfml_event.mouseButton.y)};
                const engine::core::MouseButton button = backends::fromSf(sfml_event.mouseButton.button);
                events::MouseButtonReleased event{position, button};
                event_callback_(event);
                break;
            }

            default:
            {
                
            }
        }
    }
    
}

// TODO temporary solution while moving towards hiding sfml2 to backend
sf::RenderWindow& Window::getNativeHandle() const
{
    return impl_->window_;
};

}  // namespace engine
