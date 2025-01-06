#include "gui/Panel.hpp"

#include "gui/StyleSheet.hpp"

namespace gui
{

Panel::Panel()
{
    backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
    backgroundShape_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
    backgroundShape_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());
}

void Panel::onSizeChange()
{
    backgroundShape_.setSize(getSize());
}

void Panel::onPositionChange()
{
    backgroundShape_.setPosition(Component::getGlobalPosition());
}

void Panel::onRender(sf::RenderTexture& renderWindow)
{
    renderWindow.draw(backgroundShape_);
}

void Panel::enable()
{
    backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
}

void Panel::disable()
{
    backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
}

}  // namespace gui
