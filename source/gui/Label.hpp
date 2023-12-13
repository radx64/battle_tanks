#ifndef GUI_LABEL_HPP_
#define GUI_LABEL_HPP_

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui
{

class Label : public Component
{
public:
    Label(const std::string_view& text);

    void onRender(sf::RenderTexture& renderWindow) override;
    void setText(const sf::String& text);

protected:
    sf::Text text_;
};

}  // namespace gui

#endif  // GUI_LABEL_HPP_