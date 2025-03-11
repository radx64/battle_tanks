#include "gui/Button.hpp"

#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{

std::unique_ptr<Button> Button::create(const std::string_view& text)
{
    return std::unique_ptr<Button>{new Button{text}};
}

Button::Button(const std::string_view& text)
{
    enableFocus();
    auto style = BasicStyleSheetFactory::instance();
    backgroundShape_.setFillColor(style.getWindowColor());
    backgroundShape_.setOutlineColor(style.getOutlineColor());
    backgroundShape_.setOutlineThickness(style.getOutlineThickness());
    backgroundShape_.setPosition(getGlobalPosition());
    backgroundShape_.setSize(Component::getSize());

    auto textPtr = gui::Label::create(text);
    text_ = textPtr.get();
    text_->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
    addChild(std::move(textPtr));
}

void Button::onSizeChange()
{
    auto buttonSize = getSize();
    backgroundShape_.setSize(buttonSize);
    text_->setSize(buttonSize);
}

void Button::onPositionChange()
{
    backgroundShape_.setPosition(Component::getGlobalPosition());
}

void Button::setText(const std::string_view& text)
{
    text_->setText(text);
    text_->setSize(getSize());
}

void Button::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(backgroundShape_);
}

void Button::onClick(std::function<void()> onClickCallback)
{
    onClick_ = onClickCallback;
}

EventStatus Button::on(const event::MouseEntered&)
{
    backgroundShape_.setFillColor(sf::Color(255,0,0,255));
    return gui::EventStatus::Consumed;
}
EventStatus Button::on(const event::MouseLeft&)
{
    backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
    return gui::EventStatus::Consumed;
}

EventStatus Button::on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent)
{
    if (not isFocused()) return gui::EventStatus::NotConsumed;

    auto& key = keyboardKeyPressedEvent.key;

    if (key == gui::event::Key::Space || key == gui::event::Key::Enter)
    {
        backgroundShape_.setFillColor(sf::Color(0,255,0,255));

        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus Button::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{    
    if (not isFocused()) return gui::EventStatus::NotConsumed;
    
    auto& key = keyboardKeyReleasedEvent.key;

    if (key == gui::event::Key::Space || key == gui::event::Key::Enter)
    {
        backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
        
        if (onClick_) onClick_();
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus Button::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (not isVisible_) return gui::EventStatus::NotConsumed;

    auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};
    bool isLeftClicked = mouseButtonPressedEvent.button == gui::event::MouseButton::Left;

    if (isLeftClicked and wasMouseInside() and backgroundShape_.getGlobalBounds().contains(mousePosition))
    {
        // wasMouseInside() call is added
        // due to nondeterministic behaviour of mouseLeft 
        // and mouseClick events when mouse is moved quickly
        // There might be a situation that mouseLeft event is send before mouse click
        // altough click has old position stored
        // FIXME: I need to sort it out later, but at least I know the reason now

        backgroundShape_.setFillColor(sf::Color(0,255,0,255));
      
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus Button::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonReleasedEvent.position.x, mouseButtonReleasedEvent.position.y};
    bool isLeftReleased = mouseButtonReleasedEvent.button == gui::event::MouseButton::Left;

    if (isLeftReleased and backgroundShape_.getGlobalBounds().contains(mousePosition))
    {
        backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
        focus();
        if (onClick_) onClick_();  
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus Button::on(const event::FocusLost&)
{
    auto& style = BasicStyleSheetFactory::instance();
    backgroundShape_.setOutlineColor(style.getOutlineColor());
    backgroundShape_.setOutlineThickness(style.getOutlineThickness());
    return gui::EventStatus::Consumed;
}

EventStatus Button::on(const event::FocusGained&)
{
    backgroundShape_.setOutlineColor(sf::Color::Red);
    backgroundShape_.setOutlineThickness(-4.f);
    return gui::EventStatus::Consumed;
}

}  // namespace gui
