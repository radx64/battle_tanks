#include "engine/KeyboardHandler.hpp"
#include "engine/KeyboardReceiver.hpp"

#include <unordered_map>
#include <vector>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace engine { class KeyboardReceiver; }

namespace engine
{

// This class acts as a buffer as it only for key presses whenever
// key event is pulled from sfml event queue
// Also it does nice dispatching to receivers

void KeyboardHandler::subscribe(const std::vector<sf::Keyboard::Key>& keys, KeyboardReceiver* receiver)
{
    for (const auto key : keys)
    {
        keys_with_receivers_[key].push_back(receiver);
    }

    receiver->attach(this);
}

void KeyboardHandler::unsubscribe(KeyboardReceiver* receiver)
{
    for(auto key_with_receivers_it = std::begin(keys_with_receivers_); key_with_receivers_it != std::end(keys_with_receivers_);)
    {
        auto& receivers = key_with_receivers_it->second;
        std::remove(receivers.begin(), receivers.end(), receiver);

        if (receivers.size() == 0)
        {
            key_with_receivers_it = keys_with_receivers_.erase(key_with_receivers_it);
        }
        else
        {
            ++key_with_receivers_it;
        }
    }
}

void KeyboardHandler::handleKeyPressed(const sf::Event::KeyEvent& event)
{
    const auto key = event.code;
    keys_states_[key] = true;

    auto receivers_for_key = keys_with_receivers_.find(key);

    if (receivers_for_key != keys_with_receivers_.end())
    {
        for (auto* receiver : receivers_for_key->second)
        {
            receiver->onKeyPressed(key);
        }
    }
}

void KeyboardHandler::handleKeyReleased(const sf::Event::KeyEvent& event)
{
    const auto key = event.code;
    keys_states_[key] = false;

    auto receivers_for_key = keys_with_receivers_.find(key);

    if (receivers_for_key != keys_with_receivers_.end())
    {
        for (auto* receiver : receivers_for_key->second)
        {
            receiver->onKeyReleased(key);
        }
    }
}

bool KeyboardHandler::isKeyPressed(sf::Keyboard::Key key)
{
    auto iter = keys_states_.find(key);
    if (iter != keys_states_.end())
        return iter->second;
    else
    {
        return false;
    }
}
    
}  // namespace engine
