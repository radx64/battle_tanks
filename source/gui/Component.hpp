#pragma once


#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/EventReceiver.hpp"

namespace gui
{

// TODO: Notes
// Notes to document later in some better way
// Component is basic object type for gui assembly
// Root element need to be handled by user
// Children elements are freed by parent during destruction 
// You need to render only parent to enable rendering of children

class Component : public EventReceiver
{
public:
    Component();
    virtual ~Component() = default;
    
    virtual void onRender(sf::RenderTexture& renderTexture) = 0;
    
    void render(sf::RenderTexture& renderTexture);

    virtual sf::Vector2f getSize() const;
    void setSize(const sf::Vector2f& size);

    void setVisibility(bool is_visible);
    bool isVisible();

    const sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);
    
    bool isInside(sf::Vector2f point) const;
    bool wasMouseInside() const;

    const sf::Vector2f getGlobalPosition() const;
    virtual void addChild(std::unique_ptr<Component> child);

    void disableChildrenEvents();
    void enableChildrenEvents();

    /* Mouse events */
    EventStatus receive(const event::MouseMoved& mouseMovedEvent) override final;
    EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent) override final;
    EventStatus receive(const event::MouseButtonReleased& mouseButtonReleasedEvent) override final;
    EventStatus receive(const event::MouseEntered& mouseEnteredEvent) override final;
    EventStatus receive(const event::MouseLeft& mouseLeftEvent) override final;

    /* Keyboard events */
    EventStatus receive(const event::KeyboardKeyPressed& keyboardKeyPressed) override final;
    EventStatus receive(const event::KeyboardKeyReleased& keyboardKeyReleased) override final;

protected:
    // those on methods should be overrided to define handling of mouse events
    // those are called when component receives an event
    // first forwards it to it's children
    // and then handles it itself
    // so this is some kind bubbling mechanism
    virtual EventStatus on(const event::MouseMoved& mouseMovedEvent);
    virtual EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent);
    virtual EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent);
    virtual EventStatus on(const event::MouseEntered& mouseEnteredEvent);
    virtual EventStatus on(const event::MouseLeft& mouseLeftEvent);

    virtual EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent);
    virtual EventStatus on(const event::KeyboardKeyReleased& KeyboardKeyReleased);

    virtual void onPositionChange();
    virtual void onSizeChange();
    virtual void onParentSizeChange(const sf::Vector2f& parent_size);
    
    template <typename T>
    EventStatus processEvent(const T& event, bool isConsumable);

    void updateGlobalPosition();
    size_t getChildrenCount() const;

    sf::Vector2f local_position_;   // offset from parent position
    sf::FloatRect bounds_;          // bounds box in global space position
    Component* parent_;
    bool can_children_process_events_;
    std::vector<std::unique_ptr<Component>> children_;
    bool is_visible_;
    bool was_mouse_inside_;
};

}  // namespace gui

