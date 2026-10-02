#pragma once

#include <functional>
#include <memory>
#include <string>

#include "engine/Vector2.hpp"
#include "engine/events/Event.hpp"
#include "engine/gfx/Color.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/gfx/View.hpp"

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
    void setFramerateLimit(unsigned int limit);
    void setVerticalSyncEnabled(bool enabled);
    void close();

    void clear(const engine::gfx::Color& color);
    void display();
    engine::gfx::RenderTarget& getRenderTarget();

    void setView(const engine::gfx::View& view);
    engine::gfx::View getView() const;
    const engine::gfx::View& getDefaultView() const;
    engine::Vec2f mapPixelToCoords(const engine::Vec2f& point, const engine::gfx::View& view) const;

    void pollEvents();

    using EventCallbackFn = std::function<void(engine::events::Event&)>;
    void setEventsCallback(const EventCallbackFn& callback){event_callback_ = callback;};

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    EventCallbackFn event_callback_;
};

}  // namespace engine
