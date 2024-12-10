#include "engine/input/KeyboardHandler.hpp"
#include "engine/input/KeyboardReceiver.hpp"
#include "engine/input/TextEnteredReceiver.hpp"

#include <unordered_map>
#include <vector>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace engine::input
{

void KeyboardHandler::subscribe(const std::vector<sf::Keyboard::Key>& keys, KeyboardReceiver* receiver)
{
    for (const auto key : keys)
    {
        keysWithReceivers_[key].push_back(receiver);
    }

    receiver->attach(this);
}

void KeyboardHandler::subscribe(KeyboardReceiver* receiver)
{
    anyKeyReceivers_.push_back(receiver);
    receiver->attach(this);
}

void KeyboardHandler::subscribe(TextEnteredReceiver* receiver)
{
    textEnteredReceivers_.push_back(receiver);
    receiver->attach(this);
}

void KeyboardHandler::unsubscribe(KeyboardReceiver* receiver)
{
    for(auto key_with_receivers_it = std::begin(keysWithReceivers_); key_with_receivers_it != std::end(keysWithReceivers_);)
    {
        auto& receivers = key_with_receivers_it->second;
        receivers.erase(
            std::remove(std::begin(receivers), std::end(receivers), receiver),
            std::end(receivers));

        if (receivers.size() == 0)
        {
            key_with_receivers_it = keysWithReceivers_.erase(key_with_receivers_it);
        }
        else
        {
            ++key_with_receivers_it;
        }
    }
    anyKeyReceivers_.erase(
        std::remove(std::begin(anyKeyReceivers_), std::end(anyKeyReceivers_), receiver),
        std::end(anyKeyReceivers_));
}

void KeyboardHandler::unsubscribe(TextEnteredReceiver* receiver)
{
    textEnteredReceivers_.erase(
        std::remove(std::begin(textEnteredReceivers_), std::end(textEnteredReceivers_), receiver),
        std::end(textEnteredReceivers_));
}

void KeyboardHandler::handleKeyPressed(const sf::Event::KeyEvent& event)
{
    const auto key = event.code;
    keysStates_[key] = true;

    auto receivers_for_key = keysWithReceivers_.find(key);

    if (receivers_for_key != keysWithReceivers_.end())
    {
        for (auto* receiver : receivers_for_key->second)
        {
            receiver->onKeyPressed(event);
        }
    }

    for (auto* receiver : anyKeyReceivers_)
    {
        receiver->onKeyPressed(event);
    }

}

void KeyboardHandler::handleKeyReleased(const sf::Event::KeyEvent& event)
{
    const auto key = event.code;
    keysStates_[key] = false;

    auto receivers_for_key = keysWithReceivers_.find(key);

    if (receivers_for_key != keysWithReceivers_.end())
    {
        for (auto* receiver : receivers_for_key->second)
        {
            receiver->onKeyReleased(event);
        }
    }

    for (auto* receiver : anyKeyReceivers_)
    {
        receiver->onKeyReleased(event);
    }
}

void KeyboardHandler::handleTextEntered(const sf::Event::TextEvent& event)
{
    for (auto* receiver : textEnteredReceivers_)
    {
        receiver->onTextEntered(event.unicode);
    }
}

bool KeyboardHandler::isKeyPressed(sf::Keyboard::Key key)
{
    auto iter = keysStates_.find(key);
    if (iter != keysStates_.end())
    {
        return iter->second;
    }
    else
    {
        return false;
    }
}
    
}  // namespace engine::input
