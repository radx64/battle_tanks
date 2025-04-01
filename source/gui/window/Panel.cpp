#include "gui/window/Panel.hpp"

#include "gui/StyleSheet.hpp"

namespace gui::window
{

Panel::Panel()
{
}

void Panel::addChild(std::unique_ptr<Component> child)
{
    child->setSize(getSize());
    Component::addChild(std::move(child));
}

void Panel::onSizeChange()
{
    for (auto& child : children_)
    {
        child->setSize(getSize());
    }
}

void Panel::onPositionChange()
{
}

void Panel::onRender(sf::RenderTexture& )
{
}

void Panel::enable()
{
}

void Panel::disable()
{
}

}  // namespace gui::window
