#pragma once

#include <functional>
#include <memory>
#include <string>

#include "engine/Vector2.hpp"
#include "engine/events/Event.hpp"

// TODO Hack, see below 
namespace sf { class RenderWindow; }

namespace engine
{

struct WindowProperties
{
    std::string title;
    engine::Vector2i size;

    WindowProperties(const std::string& title = "Engine", 
        const engine::Vector2i& size = engine::Vector2i{1920, 1000})
    : title(title)
    , size(size)
    {}
};

class Window
{
public:
    Window(const WindowProperties& properties);
    ~Window();

    void setTitle(const std::string& title);
    void setSize(const engine::Vector2i& size);
    void setPosition(const engine::Vector2i& position);
    void setKeyRepeatEnabled(bool enabled);
    void close();

    void pollEvents();

    using EventCallbackFn = std::function<void(engine::events::Event&)>;
    void setEventsCallback(const EventCallbackFn& callback){event_callback_ = callback;};

    // TODO temporary solution while moving towards hiding sfml2 to backend
    sf::RenderWindow& getNativeHandle() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    EventCallbackFn event_callback_;
};

}  // namespace engine
