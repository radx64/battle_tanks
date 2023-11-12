#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include <functional>

#include <SFML/Graphics.hpp>

#include "gui/Button.hpp"
#include "gui/Component.hpp"
#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"

#include <iostream>

constexpr auto TOP_BAR_WIDTH = 20.f;
constexpr auto RESIZE_THINGY_SIZE = 20.f;

namespace gui { class Label; }
namespace gui
{

class WindowPanel : public Component
{
public:
    WindowPanel()
    : style_(BasicStyleSheetFactory::create())
    {
        background_.setFillColor(style_.getInactiveWindowColor());
        background_.setOutlineColor(style_.getOutlineColor());
        background_.setOutlineThickness(style_.getOutlineThickness());
    }

    void onParentSizeChange(const sf::Vector2f& parent_size) override
    {
        setSize(parent_size);
    }

    void onParentPositionChange(const sf::Vector2f& parent_position) override
    {
        static_cast<void>(parent_position);
        setPosition(sf::Vector2f{0.f, 0.f}, alignment_);
    }

    // TODO: add some onSetSizeChange to Component class
    // to not have to call base class method every time overriding
    // setSize could be even not virtual then
    void setSize(const sf::Vector2f& size) override
    {
        static_cast<void>(size);

        auto panel_size = parent_->getSize();
        panel_size.y = panel_size.y - TOP_BAR_WIDTH - RESIZE_THINGY_SIZE;

        Component::setSize(panel_size);
        auto background_size = panel_size;
        background_.setSize(background_size);
    }

    // TODO: simmilar as TODO for setSize method
    void setPosition(const sf::Vector2f& position, const Alignment alignment) override
    {
        static_cast<void>(position);
        auto panel_position = sf::Vector2f{0.f, TOP_BAR_WIDTH};

        //FIXME position should be offset by top bar
        Component::setPosition(panel_position, alignment);
        background_.setPosition(Component::getGlobalPosition());
    }

    void onRender(sf::RenderWindow& renderWindow) override
    {
        renderWindow.draw(background_);
    }

    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked) override
    {
        static_cast<void>(mousePosition);
        static_cast<void>(isLeftClicked);
        return false;
    }

    void onFocus()
    {
        background_.setFillColor(style_.getWindowColor());
    }

    void onFocusLost()
    {
        background_.setFillColor(style_.getInactiveWindowColor());
    }

protected:
    sf::RectangleShape background_;
    gui::StyleSheet style_;
};

class TopBar : public Component
{
public:
    TopBar()
    : style_(BasicStyleSheetFactory::create())
    {
        top_bar_.setOutlineColor(style_.getOutlineColor());
        top_bar_.setOutlineThickness(style_.getOutlineThickness());

        // TODO add layout component for button and title text
        auto close_button = std::make_unique<gui::Button>("X");
        close_button->setPosition(sf::Vector2f(0.f, 0.f), gui::Alignment::LEFT);
        close_button->setSize(sf::Vector2f(20.f, TOP_BAR_WIDTH));
        close_button->onClick([this]()
        {
            if (closeButtonAction_) closeButtonAction_();
        });
        addChild(std::move(close_button));

        auto title_text = std::make_unique<gui::Label>("");
        title_text_handle_ = title_text.get();
        this->addChild(std::move(title_text));
    }

    void closeButtonAction(std::function<void()> closeButtonAction)
    {
        closeButtonAction_ = closeButtonAction;
    }

    void setTitle(const std::string_view& text)
    {
        title_text_handle_->setText(text.data());
    }

    void onRender(sf::RenderWindow& renderWindow) override
    {
        renderWindow.draw(top_bar_);
    }
    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked) override
    {
        static_cast<void>(mousePosition);
        static_cast<void>(isLeftClicked);
        return false;
    }

    void onParentSizeChange(const sf::Vector2f& parent_size) override
    {
        //FIXME this is probably wrong as only renderable has proper size
        setSize(parent_size);
    }

    void onParentPositionChange(const sf::Vector2f& parent_position) override
    {
        static_cast<void>(parent_position);
        setPosition(sf::Vector2f{0.f, 0.f}, alignment_);
    }

    // FIXME component size should be same as renderable
    void setSize(const sf::Vector2f& size) override
    {
        Component::setSize(size);
        auto top_bar_size = size;
        top_bar_size.y = TOP_BAR_WIDTH;
        top_bar_.setSize(top_bar_size);
    }

    void setPosition(const sf::Vector2f& position, const Alignment alignment) override
    {
        Component::setPosition(position, alignment);
        top_bar_.setPosition(Component::getGlobalPosition());

        const auto window_size = getSize();
        sf::Vector2f tile_text_position {window_size.x/2.f, TOP_BAR_WIDTH / 2.f};
        title_text_handle_->setPosition(tile_text_position, gui::Alignment::CENTERED);
    }

    void onFocus()
    {
        top_bar_.setFillColor(style_.getTopBarWindowColor());
    }

    void onFocusLost()
    {
        top_bar_.setFillColor(style_.getInactiveTopBarWindowColor());
    }

    bool isInside(const sf::Vector2f point)
    {
        return top_bar_.getGlobalBounds().contains(point);
    }

protected:
    gui::Label* title_text_handle_;
    sf::RectangleShape top_bar_;
    gui::StyleSheet style_;
    std::function<void()> closeButtonAction_;
};

class BottomBar : public Component
{
public:
    BottomBar()
    : style_(BasicStyleSheetFactory::create())
    {
        background_.setFillColor(style_.getInactiveWindowColor());
        background_.setOutlineColor(style_.getOutlineColor());
        background_.setOutlineThickness(style_.getOutlineThickness());

        resize_thingy_.setOutlineColor(style_.getOutlineColor());
        resize_thingy_.setOutlineThickness(style_.getOutlineThickness());
        resize_thingy_.setFillColor(style_.getInactiveWindowColor());

        resize_thingy_.setSize(sf::Vector2f{RESIZE_THINGY_SIZE, RESIZE_THINGY_SIZE});
    }

    void onRender(sf::RenderWindow& renderWindow) override
    {
        renderWindow.draw(background_);
        renderWindow.draw(resize_thingy_);
    }
    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked) override
    {
        static_cast<void>(mousePosition);
        static_cast<void>(isLeftClicked);
        return false;
    }

    void onParentSizeChange(const sf::Vector2f& parent_size) override
    {
        //FIXME this is probably wrong as only renderable has proper size
        setSize(parent_size);
    }

    void onParentPositionChange(const sf::Vector2f& parent_position) override
    {
        static_cast<void>(parent_position);
        setPosition(sf::Vector2f{0.f, 0.f}, alignment_);
    }

    // FIXME component size should be same as renderable
    void setSize(const sf::Vector2f& size) override
    {
        Component::setSize(size);
        auto bottom_bar_size = size;
        bottom_bar_size.y = RESIZE_THINGY_SIZE;
        background_.setSize(bottom_bar_size);
    }

    void setPosition(const sf::Vector2f& position, const Alignment alignment) override
    {
        Component::setPosition(position, alignment);

        auto bottom_bar_position = sf::Vector2f{};
        
        bottom_bar_position.x = position.x;
        bottom_bar_position.y = Component::getSize().y - RESIZE_THINGY_SIZE;

        background_.setPosition(Component::getGlobalPosition()+ bottom_bar_position);

        resize_thingy_.setPosition(
            Component::getGlobalPosition() 
            + Component::getSize() 
            - sf::Vector2f{RESIZE_THINGY_SIZE, RESIZE_THINGY_SIZE});
    }

    void onFocus()
    {
        background_.setFillColor(style_.getWindowColor());
    }

    void onFocusLost()
    {
        background_.setFillColor(style_.getInactiveWindowColor());
    }

    bool isInsideResizeThingy(const sf::Vector2f point)
    {
        return resize_thingy_.getGlobalBounds().contains(point);
    }


protected:
    sf::RectangleShape resize_thingy_;
    sf::RectangleShape background_;
    gui::StyleSheet style_;
};

class Window : public Component
{
public:
    Window();

    void setSize(const sf::Vector2f& size) override;
    void setTitle(const std::string_view& text);
    bool isInside(const sf::Vector2f point);
    void onRender(sf::RenderWindow& renderWindow) override;
    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked) override;
    void close();
    bool isDead() const;
    void focus();
    void defocus();
    bool isFocused() const;

    void addComponent(std::unique_ptr<Component> component)
    {
        window_panel_handle_->addChild(std::move(component));
    }

protected:
    enum class State
    {
        Idle,
        Dragging,
        Resizing
    };

    bool isInsideTopBar(const sf::Vector2f point);
    bool isInsideResizeThingy(const sf::Vector2f point);
    bool isState(const Window::State& state);
    void updateResizeThingy();

    bool killed_;
    bool focused_;
    State state_;
    sf::Vector2f dragging_offset_;
    gui::StyleSheet style_;

    //TODO: weak_ptrs maybe?
    gui::TopBar* top_bar_handle_;
    gui::WindowPanel* window_panel_handle_;
    gui::BottomBar* bottom_bar_handle_;
};

}  // namespace gui

#endif  // GUI_WINDOW_HPP_