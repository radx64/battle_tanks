#ifndef GUI_ELEMENT_HPP_
#define GUI_ELEMENT_HPP_

#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Alignment.hpp"

namespace gui
{

// Notes to document later in some better way
// Component is basic object type for gui assembly
// Root element need to be handled by user
// Children elements are freed by parent during destruction 
// You need to render only parent to enable rendering of children

class Component
{
public:
    Component();
    virtual ~Component() = default;

    virtual void onRender(sf::RenderWindow& renderWindow) = 0;
    virtual bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked) = 0;
    void render(sf::RenderWindow& renderWindow);
    bool update(const sf::Vector2f& mousePosition, bool isLeftClicked);

    virtual sf::Vector2f getSize();
    virtual void setSize(const sf::Vector2f& position);
    void setVisibility(bool is_visible);
    bool isVisible();
    const sf::Vector2f getPosition() const;
    virtual void setPosition(const sf::Vector2f& position, const Alignment alignment);
    const sf::Vector2f getGlobalPosition() const;
    void addChild(std::unique_ptr<Component> child);

protected:
    void updateGlobalPosition();
    sf::Vector2f local_position_;   // offset from parent position
    sf::FloatRect bounds_;          // bounds box in global space position
    Alignment alignment_;
    Component* parent_;
    std::vector<std::unique_ptr<Component>> children_;
    bool is_visible_;
};

}  // namespace gui

#endif  // GUI_ELEMENT_HPP_