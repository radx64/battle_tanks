#include "gui/Button.hpp"

#include "gui/Label.hpp"
#include "gui/TextureLibrary.hpp"

namespace
{
    sf::Vector2f getScalingFactor(const sf::Vector2u& textureSize, const sf::Vector2f& objectSize)
    {
        return {objectSize.x / textureSize.x, objectSize.y / textureSize.y};
    }

    gui::FramedSprite::LayoutConfig buildLayoutConfig(const sf::Vector2f& cornerSizes, const gui::FramedSprite::LayoutConfig::UVs& uvs)
    {
        gui::FramedSprite::LayoutConfig layoutConfig{
            .cornerSizes = 
            {
                .topLeft        = {cornerSizes.x, cornerSizes.y},
                .bottomRight    = {cornerSizes.x, cornerSizes.y}
            },
            .uvs = uvs
        };

        return layoutConfig;
    } 

    gui::FramedSprite::LayoutConfig::UVs buildUVsForButtonTexture()
    {
        return gui::FramedSprite::LayoutConfig::UVs
        {
            .topLeft        = {0.0f,   0.0f,   8.0f, 8.0f},
            .topRight       = {182.0f, 0.0f,   8.0f, 8.0f},
            .bottomLeft     = {0.0f,   54.0f,  8.0f, 8.0f},
            .bottomRight    = {182.0f, 54.0f,  8.0f, 8.0f},
        };
    }

    gui::FramedSprite::LayoutConfig buildLayoutConfigForButtonTexture()
    {
        static auto layout = buildLayoutConfig({10.f, 10.f}, buildUVsForButtonTexture());
        return layout;
    }

}  // namespace

namespace gui
{

ButtonBase::ButtonBase()
: background_ {buildLayoutConfigForButtonTexture()}
, hoverTexture_{TextureLibrary::instance().get("red_button_hover")}
, focusTexture_{TextureLibrary::instance().get("red_button_focus")}
, normalTexture_{TextureLibrary::instance().get("red_button_normal")}
, pressedTexture_{TextureLibrary::instance().get("red_button_pressed")}
, state_{State::Normal}
{
    enableFocus();
    background_.setTexture(normalTexture_);
}

void ButtonBase::onSizeChange()
{
    background_.setSize(getSize());
}

void ButtonBase::onPositionChange()
{
    background_.setPosition(Component::getGlobalPosition());
}

void ButtonBase::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
}

void ButtonBase::onClick(std::function<void()> onClickCallback)
{
    onClick_ = onClickCallback;
}

EventStatus ButtonBase::on(const event::MouseEntered&)
{
    state_ = State::Hover;
    updateTexture();

    return gui::EventStatus::Consumed;
}
EventStatus ButtonBase::on(const event::MouseLeft&)
{
    state_ = State::Normal;
    updateTexture();

    return gui::EventStatus::Consumed;
}

EventStatus ButtonBase::on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent)
{
    if (not isFocused()) return gui::EventStatus::NotConsumed;

    auto& key = keyboardKeyPressedEvent.key;

    if (key == gui::event::Key::Space || key == gui::event::Key::Enter)
    {
        state_ = State::Pressed;
        updateTexture();

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
        state_ = State::Normal;
        updateTexture();
        
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

    if (isLeftClicked and wasMouseInside() and isInside(mousePosition))
    {
        // wasMouseInside() call is added
        // due to nondeterministic behaviour of mouseLeft 
        // and mouseClick events when mouse is moved quickly
        // There might be a situation that mouseLeft event is send before mouse click
        // altough click has old position stored
        // FIXME: I need to sort it out later, but at least I know the reason now

        state_ = State::Pressed;
        updateTexture();

      
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonReleasedEvent.position.x, mouseButtonReleasedEvent.position.y};
    bool isLeftReleased = mouseButtonReleasedEvent.button == gui::event::MouseButton::Left;

    if (isLeftReleased and isInside(mousePosition))
    {
        focus();

        state_ = State::Hover;
        updateTexture();

        if (onClick_) onClick_();  
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::FocusLost&)
{
    updateTexture();
    return gui::EventStatus::Consumed;
}

EventStatus ButtonBase::on(const event::FocusGained&)
{
    updateTexture();
    return gui::EventStatus::Consumed;
}

void ButtonBase::updateTexture()
{
    switch (state_)
    {
        case State::Normal:
            if (isFocused())
            {
                background_.setTexture(focusTexture_);
            }
            else
            {
                background_.setTexture(normalTexture_);
            }
            break;
        case State::Hover:
            background_.setTexture(hoverTexture_);
            break;
        case State::Pressed:
            background_.setTexture(pressedTexture_);
            break;
    }
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
    setIcon(icon);
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
