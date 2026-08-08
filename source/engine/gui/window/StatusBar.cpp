#include "engine/gui/window/StatusBar.hpp"

#include "engine/gui/window/Config.hpp"
#include "engine/gui/TextureLibrary.hpp"

namespace engine::gui::window
{
StatusBar::StatusBar()
: resize_gadget_texture_{TextureLibrary::instance().get("window_resize_handle")}
{
    resize_gadget_.setTexture(resize_gadget_texture_);
}

void StatusBar::onRender(sf::RenderTexture&render_window)
{
   render_window.draw(resize_gadget_);
}

void StatusBar::onSizeChange()
{
    auto statusBarHeight = getSize().y;
    
    resize_gadget_.setPosition(
        Widget::getGlobalPosition() + Widget::getSize()
        - sf::Vector2f{window::config::RESIZE_BOX_SIZE, statusBarHeight}); 
}

void StatusBar::onPositionChange()
{
    auto statusBarHeight = getSize().y;

    resize_gadget_.setPosition(
        Widget::getGlobalPosition() + Widget::getSize()
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

}  // namespace engine::gui::window
