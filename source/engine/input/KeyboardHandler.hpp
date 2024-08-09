#pragma once
#include <unordered_map>
#include <vector>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace engine::input { class KeyboardReceiver; }
namespace engine::input { class TextEnteredReceiver; }

namespace engine::input
{

// This class acts as a buffer as it only for key presses whenever
// key event is pulled from sfml event queue
// Also it does nice dispatching to receivers
class KeyboardHandler
{
public:
    /* Subscribe for specifc key*/
    void subscribe(const std::vector<sf::Keyboard::Key>& keys, KeyboardReceiver* receiver);
    /* Subscribe for any key*/
    void subscribe(KeyboardReceiver* receiver);
    void subscribe(TextEnteredReceiver* receiver);

    void unsubscribe(KeyboardReceiver* receiver);
    void unsubscribe(TextEnteredReceiver* receiver);
    void handleKeyPressed(const sf::Event::KeyEvent& event);
    void handleKeyReleased(const sf::Event::KeyEvent& event);
    void handleTextEntered(const sf::Event::TextEvent& event);
    bool isKeyPressed(sf::Keyboard::Key key);

protected:
    std::unordered_map<sf::Keyboard::Key, std::vector<KeyboardReceiver*>> keys_with_receivers_;
    std::vector<KeyboardReceiver*> any_key_receivers_;
    std::vector<TextEnteredReceiver*> text_entered_receivers_;
    std::unordered_map<sf::Keyboard::Key, bool> keys_states_; // key, is pressed
};

    
}  // namespace engine::input

