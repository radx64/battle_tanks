#pragma once
#include <unordered_map>
#include <vector>

#include "engine/core/KeyCode.hpp"
#include "engine/events/KeyEvent.hpp"

namespace engine::input { class KeyboardReceiver; }
namespace engine::input { class TextEnteredReceiver; }

namespace engine::input
{

// This class acts as a buffer as it only for key presses whenever
// key event is pulled from engine event queue
// Also it does nice dispatching to receivers
class KeyboardHandler
{
public:
    /* Subscribe for specifc key*/
    void subscribe(const std::vector<engine::core::KeyCode>& keys, KeyboardReceiver* receiver);
    /* Subscribe for any key*/
    void subscribe(KeyboardReceiver* receiver);
    void subscribe(TextEnteredReceiver* receiver);

    void unsubscribe(KeyboardReceiver* receiver);
    void unsubscribe(TextEnteredReceiver* receiver);
    void handleKeyPressed(const engine::events::KeyPressed& event);
    void handleKeyReleased(const engine::events::KeyReleased& event);
    void handleTextEntered(const engine::events::TextEntered& event);
    bool isKeyPressed(const engine::core::KeyCode key);

protected:
    std::unordered_map<engine::core::KeyCode, std::vector<KeyboardReceiver*>> keysWithReceivers_;
    std::vector<KeyboardReceiver*> anyKeyReceivers_;
    std::vector<TextEnteredReceiver*> textEnteredReceivers_;
    std::unordered_map<engine::core::KeyCode, bool> keysStates_; // key, is pressed
};

}  // namespace engine::input
