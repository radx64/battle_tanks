#include "gui/window/StatusBar.hpp"

#include "gui/StyleSheet.hpp"
#include "gui/window/Config.hpp"
#include "gui/TextureLibrary.hpp"

namespace gui::window
{
StatusBar::StatusBar()
: resize_gadget_texture_{TextureLibrary::instance().get("window_resize_handle")}
{
    resize_gadget_.setTexture(resize_gadget_texture_);
}

void StatusBar::onRender(sf::RenderTexture& renderWindow)
{
    renderWindow.draw(resize_gadget_);
}

void StatusBar::onSizeChange()
{
    auto statusBarHeight = getSize().y;
    
    resize_gadget_.setPosition(
        Component::getGlobalPosition() + Component::getSize()
        - sf::Vector2f{window::config::RESIZE_BOX_SIZE, statusBarHeight}); 
}

void StatusBar::onPositionChange()
{
    auto statusBarHeight = getSize().y;

    resize_gadget_.setPosition(
        Component::getGlobalPosition() + Component::getSize()
        - sf::Vector2f{window::config::RESIZE_BOX_SIZE, statusBarHeight});
}

void StatusBar::enable()
{

}
void StatusBar::disable()
{

}

bool StatusBar::isInsideResizeGadget(const sf::Vector2f point)
{
    return resize_gadget_.getGlobalBounds().contains(point);
}

}  // namespace gui::window
