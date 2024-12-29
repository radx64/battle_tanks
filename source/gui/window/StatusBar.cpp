#include "gui/window/StatusBar.hpp"

#include "gui/StyleSheet.hpp"
#include "gui/window/Config.hpp"

namespace gui::window
{
StatusBar::StatusBar()
{
    backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
    backgroundShape_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
    backgroundShape_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());

    resize_gadget_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
    resize_gadget_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());
    resize_gadget_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());

    resize_gadget_.setSize(sf::Vector2f{window::config::RESIZE_BOX_SIZE, window::config::RESIZE_BOX_SIZE});
}

void StatusBar::onRender(sf::RenderTexture& renderWindow)
{
    renderWindow.draw(backgroundShape_);
    renderWindow.draw(resize_gadget_);
}

// FIXME component size should be same as renderable
void StatusBar::onSizeChange()
{
    auto bottom_bar_size = getSize();
    bottom_bar_size.y = window::config::RESIZE_BOX_SIZE;
    backgroundShape_.setSize(bottom_bar_size);

    resize_gadget_.setPosition(
        Component::getGlobalPosition() + Component::getSize()
        - sf::Vector2f{window::config::RESIZE_BOX_SIZE, window::config::RESIZE_BOX_SIZE});
}

void StatusBar::onPositionChange()
{
    backgroundShape_.setPosition(getGlobalPosition());

    resize_gadget_.setPosition(
        Component::getGlobalPosition() + Component::getSize()
        - sf::Vector2f{window::config::RESIZE_BOX_SIZE, window::config::RESIZE_BOX_SIZE});
}

void StatusBar::enable()
{
    backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
}
void StatusBar::disable()
{
    backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
}

bool StatusBar::isInsideResizeGadget(const sf::Vector2f point)
{
    return resize_gadget_.getGlobalBounds().contains(point);
}

}  // namespace gui::window
