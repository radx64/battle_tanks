#ifndef GUI_ELEMENT_HPP_
#define GUI_ELEMENT_HPP_

#include <SFML/Graphics.hpp>

#include "gui/Alignment.hpp"

namespace gui
{

// Notes to document later in some better way
// Element is basic object type for gui assembly
// Root element need to be handled by user
// Children elements are freed by parent during destruction 
// You need to render only parent to enable rendering of children

class Element
{
public:
    Element(Element* parent)
    : global_postition_{}
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

    virtual ~Element()
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
        return global_postition_;
    }

protected:
    void add_child(Element* child)
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
            global_postition_ = parent_->get_global_position() + relative_position_ + offset;
        }
        else
        {
            global_postition_ = relative_position_ + offset;
        }

        //TODO can add check if recalculation has changed position of parent to not recalculate childeren
        for (auto& child : children_)
        {
            child->update_global_position();
        }
    }

    sf::Vector2f global_postition_;
    sf::Vector2f relative_position_;
    Alignment alignment_;
    Element* parent_;
    std::vector<Element*> children_;
    bool is_visible_;
};

}  // namespace gui

#endif  // GUI_ELEMENT_HPP_