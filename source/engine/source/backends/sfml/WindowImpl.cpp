#include "WindowImpl.hpp"

#include "backends/sfml/ToSf.hpp"

namespace engine
{

namespace
{
constexpr unsigned int ANTI_ALIASING_LEVEL = 4;
}  // namespace

Window::Impl::Impl(const WindowProperties& properties)
: renderWindow_(sf::VideoMode(properties.size.x, properties.size.y, 32), properties.title,
    sf::Style::Default, sf::ContextSettings(0, 0, ANTI_ALIASING_LEVEL))
, renderTarget_(std::make_unique<backends::sfml::SfmlRenderTarget>(renderWindow_))
{
    renderWindow_.setKeyRepeatEnabled(false);
    renderWindow_.setPosition(sf::Vector2i{0, 0});
    syncDefaultView();
}

void Window::Impl::syncDefaultView()
{
    defaultView_ = fromSf(renderWindow_.getDefaultView());
}

}  // namespace engine
