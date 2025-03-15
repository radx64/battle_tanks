#include "gui/Button.hpp"

#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"
#include "gui/TextureLibrary.hpp"

namespace
{
    sf::Vector2f getScalingFactor(const sf::Vector2u& textureSize, const sf::Vector2f& objectSize)
    {
        return {objectSize.x / textureSize.x, objectSize.y / textureSize.y};
    }
}  // namespace

namespace gui
{

ButtonBase::ButtonBase()
: hoverTexture_{TextureLibrary::instance().get("red_button_hover")}
, normalTexture_{TextureLibrary::instance().get("red_button_normal")}
, pressedTexture_{TextureLibrary::instance().get("red_button_pressed")}
{
    enableFocus();
    auto style = BasicStyleSheetFactory::instance();
    backgroundShape_.setFillColor(style.getWindowColor());
    backgroundShape_.setOutlineColor(style.getOutlineColor());
    backgroundShape_.setOutlineThickness(style.getOutlineThickness());
    backgroundShape_.setPosition(getGlobalPosition());
    backgroundShape_.setSize(Component::getSize());

    background_.setTexture(normalTexture_);
}

void ButtonBase::onSizeChange()
{
    backgroundShape_.setSize(getSize());

    auto backgroundTextureSize = background_.getTexture()->getSize();
    auto buttonSize = getSize();
    background_.setScale(getScalingFactor(backgroundTextureSize, buttonSize));
}

void ButtonBase::onPositionChange()
{
    backgroundShape_.setPosition(Component::getGlobalPosition());

    background_.setPosition(Component::getGlobalPosition());
}

void ButtonBase::onRender(sf::RenderTexture& renderTexture)
{
    //renderTexture.draw(backgroundShape_);
    renderTexture.draw(background_);
}

void ButtonBase::onClick(std::function<void()> onClickCallback)
{
    onClick_ = onClickCallback;
}

EventStatus ButtonBase::on(const event::MouseEntered&)
{
    backgroundShape_.setFillColor(sf::Color(255,0,0,255));

    background_.setTexture(hoverTexture_);

    return gui::EventStatus::Consumed;
}
EventStatus ButtonBase::on(const event::MouseLeft&)
{
    backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());

    background_.setTexture(normalTexture_);

    return gui::EventStatus::Consumed;
}

EventStatus ButtonBase::on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent)
{
    if (not isFocused()) return gui::EventStatus::NotConsumed;

    auto& key = keyboardKeyPressedEvent.key;

    if (key == gui::event::Key::Space || key == gui::event::Key::Enter)
    {
        backgroundShape_.setFillColor(sf::Color(0,255,0,255));

        background_.setTexture(pressedTexture_);

        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{    
    if (not isFocused()) return gui::EventStatus::NotConsumed;
    
    auto& key = keyboardKeyReleasedEvent.key;

    if (key == gui::event::Key::Space || key == gui::event::Key::Enter)
    {
        backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());

        background_.setTexture(normalTexture_);
        
        if (onClick_) onClick_();
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
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

        background_.setTexture(pressedTexture_);
      
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonReleasedEvent.position.x, mouseButtonReleasedEvent.position.y};
    bool isLeftReleased = mouseButtonReleasedEvent.button == gui::event::MouseButton::Left;

    if (isLeftReleased and backgroundShape_.getGlobalBounds().contains(mousePosition))
    {
        backgroundShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());

        background_.setTexture(hoverTexture_);

        focus();
        if (onClick_) onClick_();  
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::FocusLost&)
{
    auto& style = BasicStyleSheetFactory::instance();
    backgroundShape_.setOutlineColor(style.getOutlineColor());
    backgroundShape_.setOutlineThickness(style.getOutlineThickness());
    return gui::EventStatus::Consumed;
}

EventStatus ButtonBase::on(const event::FocusGained&)
{
    backgroundShape_.setOutlineColor(sf::Color::Red);
    backgroundShape_.setOutlineThickness(-4.f);
    return gui::EventStatus::Consumed;
}

std::unique_ptr<TextButton> TextButton::create(const std::string_view& text)
{
    return std::unique_ptr<TextButton>{new TextButton{text}};
}

TextButton::TextButton(const std::string_view& text)
{
    auto textPtr = gui::Label::create(text);
    text_ = textPtr.get();
    text_->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
    addChild(std::move(textPtr));
}

void TextButton::setText(const std::string_view& text)
{
    text_->setText(text);
    text_->setSize(getSize());
}

void TextButton::onSizeChange()
{
    ButtonBase::onSizeChange();
    text_->setSize(getSize());
}

std::unique_ptr<IconButton> IconButton::create(const sf::Texture& icon)
{
    return std::unique_ptr<IconButton>{new IconButton{icon}};
}

IconButton::IconButton(const sf::Texture& icon)
{
    icon_.setTexture(icon);
    icon_.setPosition(getGlobalPosition());
    icon_.setTextureRect(sf::IntRect(0,0,getSize().x, getSize().y));
}

void IconButton::setIcon(const sf::Texture& icon)
{
    icon_.setTexture(icon);
    icon_.setPosition(getGlobalPosition());
    icon_.setTextureRect(sf::IntRect(0,0,getSize().x, getSize().y));   
}

void IconButton::onRender(sf::RenderTexture& renderTexture)
{
    ButtonBase::onRender(renderTexture);
    renderTexture.draw(icon_);
}

void IconButton::onSizeChange()
{
    ButtonBase::onSizeChange();

    sf::Vector2u textureSize = icon_.getTexture()->getSize();
    sf::Vector2f buttonSize = getSize();
    sf::Vector2f scale = getScalingFactor(textureSize, buttonSize);

    icon_.setScale(scale);
    icon_.setTextureRect(sf::IntRect(0,0,buttonSize.x / scale.x, buttonSize.y / scale.y));
}

void IconButton::onPositionChange()
{
    ButtonBase::onPositionChange();
    icon_.setPosition(getGlobalPosition());
}

}  // namespace gui
