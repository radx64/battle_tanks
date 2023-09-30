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
            parent_->add_child(this);
        }
    }

    virtual ~Component()
    {
        for (auto* child : children_)
        {
            delete child;
        }        
    }

    virtual void on_render(sf::RenderWindow& renderWindow) = 0;

    void render(sf::RenderWindow& renderWindow)
    {
        if (!is_visible_) return;
        on_render(renderWindow);
        for (auto child : children_)
        {
            child->render(renderWindow); 
        }
    }

    virtual bool on_mouse_update(const sf::Vector2f& mousePosition, bool isLeftClicked) = 0;

    bool update(const sf::Vector2f& mousePosition, bool isLeftClicked)
    {
        bool was_mouse_event_processed {false};
        was_mouse_event_processed = on_mouse_update(mousePosition, isLeftClicked);
        for (auto child : children_)
        {
            if (child->update(mousePosition, isLeftClicked)) was_mouse_event_processed = true;
        }
        return was_mouse_event_processed;
    }

    virtual float get_width() = 0;

    virtual void set_position(const sf::Vector2f& position, const Alignment alignment)
    {
        if (relative_position_ == position) return;
        relative_position_ = position;
        alignment_ = alignment;
        update_global_position();
    }

    void set_visibility(bool is_visible)
    {
        is_visible_ = is_visible;
    }

    const sf::Vector2f& get_position() const
    {
        return relative_position_;
    }

    const sf::Vector2f& get_global_position() const
    {
        return global_position_;
    }

protected:
    void add_child(Component* child)
    {
        auto found = std::find(children_.cbegin(), children_.cend(), child);
        if (found != children_.cend())
        {
            return;
        }

        children_.push_back(child);
    }

    void update_global_position()
    {
        sf::Vector2f offset{0.0f, 0.0f};

        switch (alignment_)
        {
            case (gui::Alignment::LEFT)     : offset.x = 0.0f; break;
            case (gui::Alignment::RIGHT)    : offset.x = - get_width(); break;
            case (gui::Alignment::CENTERED) : offset.x = - get_width() / 2.0f; break;
        }

        if (parent_)
        {
            global_position_ = parent_->get_global_position() + relative_position_ + offset;
        }
        else
        {
            global_position_ = relative_position_ + offset;
        }

        //TODO can add check if recalculation has changed position of parent to not recalculate childeren
        for (auto& child : children_)
        {
            child->update_global_position();
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