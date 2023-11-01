#ifndef GUI_ELEMENT_HPP_
#define GUI_ELEMENT_HPP_

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
    Component(Component* parent)
    : global_position_{}
    , relative_position_{}
    , alignment_ {}
    , parent_{parent}
    , children_ {}
    , is_visible_ {true}
    {
        if (parent != nullptr)
        {
            parent_->addChild(this);
        }
    }

    virtual ~Component()
    {
        for (auto* child : children_)
        {
            delete child;
        }        
    }

    virtual void onRender(sf::RenderWindow& renderWindow) = 0;

    void render(sf::RenderWindow& renderWindow)
    {
        if (!is_visible_) return;
        onRender(renderWindow);
        for (auto child : children_)
        {
            child->render(renderWindow); 
        }
    }

    virtual bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked) = 0;

    bool update(const sf::Vector2f& mousePosition, bool isLeftClicked)
    {
        bool was_mouse_event_processed {false};
        was_mouse_event_processed = onMouseUpdate(mousePosition, isLeftClicked);
        for (auto child : children_)
        {
            if (child->update(mousePosition, isLeftClicked)) was_mouse_event_processed = true;
        }
        return was_mouse_event_processed;
    }

    virtual float getWidth() = 0;

    virtual void setPosition(const sf::Vector2f& position, const Alignment alignment)
    {
        if (relative_position_ == position) return;
        relative_position_ = position;
        alignment_ = alignment;
        updateGlobalPosition();
    }

    void setVisibility(bool is_visible)
    {
        is_visible_ = is_visible;
    }

    const sf::Vector2f& getPosition() const
    {
        return relative_position_;
    }

    const sf::Vector2f& getGlobalPosition() const
    {
        return global_position_;
    }

protected:
    void addChild(Component* child)
    {
        auto found = std::find(children_.cbegin(), children_.cend(), child);
        if (found != children_.cend())
        {
            return;
        }

        children_.push_back(child);
    }

    void updateGlobalPosition()
    {
        sf::Vector2f offset{0.0f, 0.0f};

        switch (alignment_)
        {
            case (gui::Alignment::LEFT)     : offset.x = 0.0f; break;
            case (gui::Alignment::RIGHT)    : offset.x = - getWidth(); break;
            case (gui::Alignment::CENTERED) : offset.x = - getWidth() / 2.0f; break;
        }

        if (parent_)
        {
            global_position_ = parent_->getGlobalPosition() + relative_position_ + offset;
        }
        else
        {
            global_position_ = relative_position_ + offset;
        }

        //TODO can add check if recalculation has changed position of parent to not recalculate childeren
        for (auto& child : children_)
        {
            child->updateGlobalPosition();
        }
    }

    sf::Vector2f global_position_;
    sf::Vector2f relative_position_;
    Alignment alignment_;
    Component* parent_;
    std::vector<Component*> children_;
    bool is_visible_;
};

}  // namespace gui

#endif  // GUI_ELEMENT_HPP_