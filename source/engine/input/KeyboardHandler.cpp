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
        keys_with_receivers_[key].push_back(receiver);
    }

    receiver->attach(this);
}

void KeyboardHandler::subscribe(KeyboardReceiver* receiver)
{
    any_key_receivers_.push_back(receiver);
    receiver->attach(this);
}

void KeyboardHandler::subscribe(TextEnteredReceiver* receiver)
{
    text_entered_receivers_.push_back(receiver);
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
    any_key_receivers_.erase(
        std::remove(std::begin(any_key_receivers_), std::end(any_key_receivers_), receiver),
        std::end(any_key_receivers_));
}

void KeyboardHandler::unsubscribe(TextEnteredReceiver* receiver)
{
    text_entered_receivers_.erase(
        std::remove(std::begin(text_entered_receivers_), std::end(text_entered_receivers_), receiver),
        std::end(text_entered_receivers_));
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
            receiver->onKeyPressed(event);
        }
    }

    for (auto* receiver : any_key_receivers_)
    {
        receiver->onKeyPressed(event);
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
            receiver->onKeyReleased(event);
        }
    }

    for (auto* receiver : any_key_receivers_)
    {
        receiver->onKeyReleased(event);
    }
}

void KeyboardHandler::handleTextEntered(const sf::Event::TextEvent& event)
{
    for (auto* receiver : text_entered_receivers_)
    {
        receiver->onTextEntered(event.unicode);
    }
}

bool KeyboardHandler::isKeyPressed(sf::Keyboard::Key key)
{
    auto iter = keys_states_.find(key);
    if (iter != keys_states_.end())
    {
        return iter->second;
    }
    else
    {
        return false;
    }
}
    
}  // namespace engine::input
