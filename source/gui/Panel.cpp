#include "gui/Panel.hpp"

#include "gui/StyleSheet.hpp"

namespace gui
{

Panel::Panel()
{
    background_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
    background_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
    background_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());
}

void Panel::onSizeChange()
{
    background_.setSize(getSize());
}


void Panel::onPositionChange()
{
    background_.setPosition(Component::getGlobalPosition());
}

void Panel::onRender(sf::RenderTexture& renderWindow)
{
    renderWindow.draw(background_);
}

void Panel::onFocus()
{
    background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
}

void Panel::onFocusLost()
{
    background_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowColor());
}

}  // namespace gui