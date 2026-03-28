#pragma once

#include <memory>
#include <source_location>

#include <SFML/Graphics.hpp>

#include "engine/Logger.hpp"
#include "gui/EventReceiver.hpp"

namespace gui
{

// TODO: Notes to document later in some better way
// Widget is basic object type for gui assembly
// Root element need to be handled by user
// Children elements are freed by parent during destruction
// You need to render only parent to enable rendering of children

class Widget : public EventReceiver
{
public:
    Widget(const std::source_location location =
               std::source_location::current());
    virtual ~Widget();

    uint32_t getId() const;
    const std::string_view getDebugName() const;
    virtual void onRender(sf::RenderTexture& renderTexture) = 0;

    void render(sf::RenderTexture& renderTexture);

    virtual sf::Vector2f getSize() const;
    void setSize(const sf::Vector2f& size);

    void setVisibility(bool isVisible);
    bool isVisible() const;

    const sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);

    bool isInside(sf::Vector2f point) const;
    bool isInside(const event::MousePosition& position) const;

    const sf::Vector2f getGlobalPosition() const;
    virtual void addChild(std::unique_ptr<Widget> child);
    virtual void removeChild(const Widget* child);
    Widget* getParent() const;
    Widget* getRoot();
    const std::vector<std::unique_ptr<Widget>>& getChildren() const;

    void focus();
    void defocus();
    bool isFocused() const;
    void defocusWithAllChildren();
    void defocusChildrenExcept(const Widget* focusedChild);
    void selectFocusedChild(Widget* focusedChild);
    bool isFocusable() const;
    void enableFocus(); // widget can receive focus
    void disableFocus();

    void disableChildrenEvents();
    void enableChildrenEvents();

    /* Mouse events */
    EventStatus receive(const event::MouseMoved& mouseMovedEvent) override final;
    EventStatus receive(const event::MouseButtonPressed& mouseButtonPressedEvent) override final;
    EventStatus receive(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override final;
    EventStatus receive(const event::MouseButtonReleased& mouseButtonReleasedEvent) override final;
    EventStatus receive(const event::MouseEntered& mouseEnteredEvent) override final;
    EventStatus receive(const event::MouseLeft& mouseLeftEvent) override final;

    /* Keyboard events */
    EventStatus receive(const event::KeyboardKeyPressed& keyboardKeyPressed) override final;
    EventStatus receive(const event::KeyboardKeyReleased& keyboardKeyReleased) override final;
    EventStatus receive(const event::TextEntered& textEntered) override final;

    /* Focus events */
    EventStatus receive(const event::FocusLost& focusLost) override final;
    EventStatus receive(const event::FocusGained& focusGained) override final;
protected:

    /*
        These on methods should be overrided to define handling
        of mouse and keyboards events
        These are called when widget receives an event
        first forwards it to it's children
        and then handles it itself
        so this is some kind bubbling mechanism
    */

    virtual EventStatus on(const event::MouseMoved& mouseMovedEvent);
    virtual EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent);
    virtual EventStatus on(const event::MouseButtonDoublePressed& mouseButtonPressedEvent);
    virtual EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent);
    virtual EventStatus on(const event::MouseEntered& mouseEnteredEvent);
    virtual EventStatus on(const event::MouseLeft& mouseLeftEvent);

    virtual EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent);
    virtual EventStatus on(const event::KeyboardKeyReleased& KeyboardKeyReleased);
    virtual EventStatus on(const event::TextEntered& textEntered);

    virtual EventStatus on(const event::FocusLost& focusLost);
    virtual EventStatus on(const event::FocusGained& focusGained);

    virtual void onPositionChange();
    virtual void onSizeChange();

    template <typename T>
    EventStatus processEvent(const T& event);

    void updateGlobalPosition();
    size_t getChildrenCount() const;

    void applyPendingOperations(); // TODO: probably is no longer needed as events are handled in different manner. Will check later when I will fix context menus.

    sf::Vector2f localPosition_;   // offset from parent position
    sf::FloatRect bounds_;         // bounds box in global space position
    Widget* parent_;
    bool childrenEventsProcessingEnabled_;
    std::vector<std::unique_ptr<Widget>> children_;

    // TODO: engine::TasksQueue can be considered to handle pending add/remove children 
    // operations instead of vector of pending operations and applyPendingOperations method 
    std::vector<std::unique_ptr<Widget>> pendingChildrenToAdd_;
    std::vector<const Widget*> pendingChildrenToRemove_;

    Widget* focusedChild_;
    bool isVisible_;
    
    bool isFocused_;
    bool isFocusable_;          // can widget get focus, if not it should still forward events to children
    bool isProcessingEvents_;   // when processing events, addChild, removeChild will be posponed to not break current childred iterators. 
    uint32_t id_;
    std::string debugName_;
    engine::Logger logger_;

public:
    void new_focus();
    void new_defocus();
    bool hasChildren() const;
    Widget* getNextChild(Widget* child);
    Widget* getNextSibling();

    Widget* getPreviousChild(Widget* child);
    Widget* getPreviousSibling();
};

}  // namespace gui
