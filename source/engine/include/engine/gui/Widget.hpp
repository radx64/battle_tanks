#pragma once

#include <memory>
#include <source_location>

#include <SFML/Graphics.hpp>

#include "engine/Logger.hpp"
#include "engine/gui/EventReceiver.hpp"

namespace engine::gui {class GUI;}

namespace engine::gui
{

// TODO: Notes to document later in some better way
// Widget is basic object type for gui assembly
// Root element need to be handled by user
// Children elements are freed by parent during destruction
// You need to render only parent to enable rendering of children

// TODO rethink if Widget should be an EventReceiver derivative
// As now GuiController does the heavy lifting maybe this is not needed anymore??
// Probably calling on(event) method would be suficient

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

    virtual engine::Vec2f getSize() const;
    void setSize(const engine::Vec2f& size);

    void setVisibility(bool isVisible);
    bool isVisible() const;

    const engine::Vec2f getPosition() const;
    void setPosition(const engine::Vec2f& position);

    bool isInside(sf::Vector2f point) const;
    bool isInside(const engine::Vec2f& position) const;

    const engine::Vec2f getGlobalPosition() const;
    virtual void addChild(std::unique_ptr<Widget> child);
    virtual void removeChild(const Widget* child);
    Widget* getParent() const;
    Widget* getRoot();
    const std::vector<std::unique_ptr<Widget>>& getChildren() const;

    bool isFocused() const;
    void defocusWithAllChildren();
    void defocusChildrenExcept(const Widget* focusedChild);

    void focus();
    void defocus();
    bool hasChildren() const;
    Widget* getNextChild(Widget* child);
    Widget* getNextSibling();

    Widget* getPreviousChild(Widget* child);
    Widget* getPreviousSibling();

    void setGui(GUI* gui);
    GUI& gui() const;

    bool isFocusable() const;
    void enableFocus(); // widget can receive focus
    void disableFocus();

    /* Mouse events */
    EventStatus receive(const engine::events::MouseMoved& mouseMovedEvent) override final;
    EventStatus receive(const engine::events::MouseButtonPressed& mouseButtonPressedEvent) override final;
    EventStatus receive(const engine::gui::events::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override final;
    EventStatus receive(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent) override final;
    EventStatus receive(const engine::gui::events::MouseEntered& mouseEnteredEvent) override final;
    EventStatus receive(const engine::gui::events::MouseLeft& mouseLeftEvent) override final;

    /* Keyboard events */
    EventStatus receive(const engine::events::KeyPressed& keyboardKeyPressed) override final;
    EventStatus receive(const engine::events::KeyReleased& keyboardKeyReleased) override final;
    EventStatus receive(const engine::events::TextEntered& textEntered) override final;

    /* Focus events */
    EventStatus receive(const engine::gui::events::FocusLost& focusLost) override final;
    EventStatus receive(const engine::gui::events::FocusGained& focusGained) override final;
protected:

    /*
        These on methods should be overrided to define handling
        of mouse and keyboards events
        These are called when widget receives an event
        first forwards it to it's children
        and then handles it itself
        so this is some kind bubbling mechanism
    */

    virtual EventStatus on(const engine::events::MouseMoved& mouseMovedEvent);
    virtual EventStatus on(const engine::events::MouseButtonPressed& mouseButtonPressedEvent);
    virtual EventStatus on(const engine::gui::events::MouseButtonDoublePressed& mouseButtonDoublePressedEvent);
    virtual EventStatus on(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent);
    virtual EventStatus on(const engine::gui::events::MouseEntered& mouseEnteredEvent);
    virtual EventStatus on(const engine::gui::events::MouseLeft& mouseLeftEvent);

    virtual EventStatus on(const engine::events::KeyPressed& keyboardKeyPressedEvent);
    virtual EventStatus on(const engine::events::KeyReleased& KeyboardKeyReleased);
    virtual EventStatus on(const engine::events::TextEntered& textEntered);

    virtual EventStatus on(const engine::gui::events::FocusLost& focusLost);
    virtual EventStatus on(const engine::gui::events::FocusGained& focusGained);

    virtual void onPositionChange();
    virtual void onSizeChange();

    template <typename T>
    EventStatus processEvent(const T& event);

    void updateGlobalPosition();
    size_t getChildrenCount() const;

    void applyPendingOperations(); // TODO: probably is no longer needed as events are handled in different manner. Will check later when I will fix context menus.

    engine::Vec2f localPosition_;  // offset from parent position
    sf::FloatRect bounds_;         // bounds box in global space position
    Widget* parent_; 
    GUI* gui_;                     // pointer to gui handling this widget
    std::vector<std::unique_ptr<Widget>> children_;

    // TODO: engine::TasksQueue can be considered to handle pending add/remove children 
    // operations instead of vector of pending operations and applyPendingOperations method 
    std::vector<std::unique_ptr<Widget>> pendingChildrenToAdd_;
    std::vector<const Widget*> pendingChildrenToRemove_;

    bool isVisible_;
    
    bool isFocused_;
    bool isFocusable_;          // can widget get focus, if not it should still forward events to children
    bool isProcessingEvents_;   // when processing events, addChild, removeChild will be posponed to not break current children iterators. 
    uint32_t id_;
    std::string debugName_;
    engine::Logger logger_;
};

}  // namespace engine::gui
