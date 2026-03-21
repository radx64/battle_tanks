#include "gui/WindowManager.hpp"

#include <iterator>

#include "gui/ContextMenu.hpp"
#include "gui/Window.hpp"

#include "gui/Debug.hpp"

namespace gui
{

WindowManager::WindowManager(const sf::Vector2f& mainWindowSize)
: activeWindow_{nullptr}
, mainWindow_{}
, logger_{"WindowManager"}
{
    mainWindow_.setSize(mainWindowSize);
    renderTexture_.create(mainWindowSize.x, mainWindowSize.y);
}

WindowManager::~WindowManager() = default;

void WindowManager::openWindow(std::unique_ptr<Window> window)
{
    window->setWindowCloseHandler([this, windowPtr = window.get()]() {
        if (activeWindow_ != windowPtr) return;

        // select next active window (if any)
        // this is done before removing window from list, so I can select next active window based on current order of windows, 
        // not based on order after closed window is removed
        activeWindow_ = windows_.size() > 1 ? std::next(windows_.begin())->get() : nullptr;

        if (activeWindow_)
        {
            activeWindow_->enable();
        }

        windowCloseHandler_? windowCloseHandler_(windowPtr) : logger_.debug("Window close handler not set");

        logger_.debug("Window closed, id: " + std::to_string(windowPtr->getId()));

        windows_.remove_if([windowPtr](const std::unique_ptr<Window>& ptr) {
            return ptr.get() == windowPtr;
        });
    });

    if (activeWindow_)
    {
        activeWindow_->disable();
    }
    activeWindow_ = window.get();
    activeWindow_->enable();
    mainWindow_.defocusWithAllChildren();
    window->setManager(this);
    windows_.push_front(std::move(window));
}

void WindowManager::openContextMenu(std::unique_ptr<ContextMenu> menu, const sf::Vector2f& globalPosition)
{
    if (!menu) return;

    menu->setCloseCallback([this](ContextMenu* menuPtr) {
        removeOverlay(menuPtr);
    });

    menu->open(globalPosition);
    addOverlay(std::move(menu));
}

void WindowManager::addOverlay(std::unique_ptr<Overlay> overlay)
{
    if (!overlay)
    {
        return;
    }

    // When an overlay is active we want to treat the UI as modal; prevent hover/interaction
    // with underlying windows while the overlay is visible.
    mainWindow_.defocusWithAllChildren();
    for (auto& window : windows_)
    {
        window->defocusWithAllChildren();
    }

    overlays_.push_back(std::move(overlay));
}

void WindowManager::removeOverlay(Overlay* overlay)
{
    overlayCloseHandler_? overlayCloseHandler_(overlay) : logger_.debug("Overlay close handler not set");

    // TODO: Right now I don't need to defer overlay removal
    // as this is the last action done from menu callback
    // But if I will need to do something after closing menu in callback, I will have to defer it, 
    // as removing overlay immediately will cause invalid memory access when callback tries to access menu after it is closed
    // Just in case I can always restore engine::taskQueue
    // Rethink that later
    overlays_.remove_if([overlay](const std::unique_ptr<Overlay>& ptr) {
        return ptr.get() == overlay;
    });
}

void WindowManager::render(sf::RenderWindow& renderWindow)
{
    renderTexture_.clear(sf::Color{0,0,0,0});

    mainWindow_.render(renderTexture_);

    for (auto window = windows_.rbegin(); window != windows_.rend(); ++window  )
    {
        (*window)->render(renderTexture_);
    }

    for (auto& overlay : overlays_)
    {
        overlay->render(renderTexture_);
    }

    renderTexture_.display();
    textureSprite_.setTexture(renderTexture_.getTexture());
    renderWindow.draw(textureSprite_);
}

MainWindow& WindowManager::mainWindow()
{
    return mainWindow_;
}

Window* WindowManager::getActiveWindow() const
{
    return activeWindow_;
}

std::vector<Window*> WindowManager::getInactiveWindows() const
{
    std::vector<Window*> inactiveWindows;
    inactiveWindows.reserve(windows_.size());
    for (const auto& window : windows_)
    {
        if (window.get() != activeWindow_)
        {
            inactiveWindows.push_back(window.get());
        }
    }
    return inactiveWindows;
}

void WindowManager::setActiveWindow(Window* window)
{
    if (activeWindow_ == window)
    {
        return;
    }

    if (activeWindow_)
    {
        activeWindow_->disable();
    }

    activeWindow_ = window;

    if (!window)
    {
        return;
    }

    window->enable();

    auto windowIterator = std::find_if(windows_.begin(), windows_.end(), [window](const std::unique_ptr<Window>& w) {
        return w.get() == window;
    });

    windows_.splice(windows_.begin(), windows_, windowIterator);
}

void WindowManager::setWindowCloseHandler(WindowCloseHandler handler)
{
    windowCloseHandler_ = handler;
}

void WindowManager::setOverlayCloseHandler(OverlayCloseHandler handler)
{
    overlayCloseHandler_ = handler;
}

Window* WindowManager::getTopWindowAtPosition(const event::MousePosition position) const
{
    for (auto& window : windows_)
    {
        if (window->isInside(position))
        {
            return window.get();
        }
    }

    return nullptr;
}

WindowManager::Overlays& WindowManager::getOverlays()
{
    return overlays_;
}

bool WindowManager::hasOverlays() const
{
    return not overlays_.empty();
}

}  // namespace gui
