#include "engine/Window.hpp"

#include <SFML/Window.hpp>

#include "engine/events/ApplicationEvent.hpp"
#include "engine/events/KeyEvent.hpp"
#include "engine/events/MouseEvent.hpp"

#include "WindowImpl.hpp"

#include "backends/sfml/ToSf.hpp"
#include "fromSf.hpp"

namespace engine
{

Window::Window(const WindowProperties& properties)
: impl_(std::make_unique<Impl>(properties))
{}

Window::~Window() = default;

void Window::setTitle(const std::string& title)
{
    impl_->renderWindow_.setTitle(title);
}

void Window::setSize(const engine::Vector2i& size)
{
    impl_->renderWindow_.setSize(sf::Vector2u(size.x, size.y));
    impl_->syncDefaultView();
}

void Window::setPosition(const engine::Vector2i& position)
{
    impl_->renderWindow_.setPosition(sf::Vector2i(position.x, position.y));
}

void Window::setKeyRepeatEnabled(bool enabled)
{
    impl_->renderWindow_.setKeyRepeatEnabled(enabled);
}

void Window::setFramerateLimit(const unsigned int limit)
{
    impl_->renderWindow_.setFramerateLimit(limit);
}

void Window::setVerticalSyncEnabled(const bool enabled)
{
    impl_->renderWindow_.setVerticalSyncEnabled(enabled);
}

void Window::close()
{
    impl_->renderWindow_.close();
}

void Window::clear(const engine::gfx::Color& color)
{
    impl_->renderTarget_->clear(color);
}

void Window::display()
{
    impl_->renderWindow_.display();
}

engine::gfx::RenderTarget& Window::getRenderTarget()
{
    return *impl_->renderTarget_;
}

void Window::setView(const engine::gfx::View& view)
{
    impl_->renderWindow_.setView(toSf(view));
}

engine::gfx::View Window::getView() const
{
    return fromSf(impl_->renderWindow_.getView());
}

const engine::gfx::View& Window::getDefaultView() const
{
    return impl_->defaultView_;
}

engine::Vec2f Window::mapPixelToCoords(const engine::Vec2f& point, const engine::gfx::View& view) const
{
    const auto mapped = impl_->renderWindow_.mapPixelToCoords(
        sf::Vector2i{static_cast<int>(point.x), static_cast<int>(point.y)},
        toSf(view));
    return engine::Vec2f{mapped.x, mapped.y};
}

void Window::pollEvents()
{
    if (!event_callback_) return;

    sf::Event sfml_event;

    while(impl_->renderWindow_.pollEvent(sfml_event))
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

}  // namespace engine
