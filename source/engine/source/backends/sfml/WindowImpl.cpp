#include "WindowImpl.hpp"

namespace engine
{

namespace
{
constexpr unsigned int ANTI_ALIASING_LEVEL = 4;
}  // namespace  


Window::Impl::Impl(const WindowProperties& properties)
: renderWindow_(sf::VideoMode(properties.size.x, properties.size.y, 32), properties.title,
    sf::Style::Default, sf::ContextSettings(0, 0, ANTI_ALIASING_LEVEL))
{
    renderWindow_.setKeyRepeatEnabled(false);
    renderWindow_.setPosition(sf::Vector2i{0, 0});
}

}  // namespace engine
