#include "gui/Button.hpp"

#include "gui/Label.hpp"
#include "gui/layout/Inset.hpp"
#include "gui/style/Sheet.hpp"
#include "gui/style/StyleFactory.hpp"
#include "gui/TextureLibrary.hpp"

namespace
{

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
        .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
        .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
        .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
        .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
    };
}

gui::FramedSprite::LayoutConfig buildLayoutConfigForButtonTexture()
{
    static auto layout = buildLayoutConfig({4.f, 4.f}, buildUVsForButtonTexture());
    return layout;
}

}  // namespace

inline sf::Texture& getTexture(const std::string_view& name)
{
    return gui::TextureLibrary::instance().get(name);
}

namespace gui
{

ButtonBase::ButtonBase()
: background_ {buildLayoutConfigForButtonTexture()}
, style_{style::StyleFactory::instance().button}
, focusTexture_{getTexture("button_focus")}
, state_{}
{

    // TODO:
    // Implement focus as a nice repeated dot border over normal/hover texture
    // as a separate layer (using texture or primitives?)

    enableFocus();
    background_.setTexture(getTexture(style_.face.texture.get(state_)));
    background_.setColor(style_.face.color.get(state_));
}

void ButtonBase::onSizeChange()
{
    background_.setSize(getSize());
}

void ButtonBase::onPositionChange()
{
    background_.setPosition(getGlobalPosition());
}

void ButtonBase::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
}

void ButtonBase::onClick(std::function<void()> onClickCallback)
{
    onClick_ = onClickCallback;
}

void ButtonBase::onMouseEnter(std::function<void()> onMouseEnterCallback)
{
    onMouseEnter_ = onMouseEnterCallback;
}

EventStatus ButtonBase::on(const event::MouseEntered&)
{
    state_.hovered = true;
    updateTexture();

    if (onMouseEnter_) onMouseEnter_();

    return EventStatus::Consumed;
}
EventStatus ButtonBase::on(const event::MouseLeft&)
{
    state_.hovered = false;
    updateTexture();

    return EventStatus::Consumed;
}

EventStatus ButtonBase::on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent)
{
    if (not isFocused()) return EventStatus::NotConsumed;

    auto& key = keyboardKeyPressedEvent.key;

    if (key == gui::event::Key::Space || key == gui::event::Key::Enter)
    {
        state_.pressed = true;
        updateTexture();

        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{    
    if (not isFocused()) return EventStatus::NotConsumed;

    auto& key = keyboardKeyReleasedEvent.key;

    if (key == gui::event::Key::Space || key == gui::event::Key::Enter)
    {
        state_.pressed = false;
        updateTexture();
        
        if (onClick_) onClick_();
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (not isVisible_) return EventStatus::NotConsumed;

    bool isLeftClicked = mouseButtonPressedEvent.button == gui::event::MouseButton::Left;

    if (isLeftClicked)
    {
        state_.pressed = true;
        updateTexture();

        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (not state_.pressed) return EventStatus::NotConsumed;

    bool isLeftReleased = mouseButtonReleasedEvent.button == gui::event::MouseButton::Left;
    if (isLeftReleased)
    {
       return  processLeftMouseClick();               
    }
    return EventStatus::NotConsumed;
}

EventStatus ButtonBase::on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    if (not isVisible_) return EventStatus::NotConsumed;
    
    bool isLeftDoubleClicked = mouseButtonDoublePressedEvent.button == gui::event::MouseButton::Left;
    if (not isLeftDoubleClicked) return EventStatus::NotConsumed;

    return processLeftMouseClick();
}

EventStatus ButtonBase::processLeftMouseClick()
{
    //focus();

    state_.pressed = false;
    updateTexture();

    if (onClick_) onClick_();
    return EventStatus::Consumed;

}

EventStatus ButtonBase::on(const event::FocusLost&)
{
    logger_.debug("Button lost focus");
    updateTexture();
    return EventStatus::Consumed;
}

EventStatus ButtonBase::on(const event::FocusGained&)
{
    logger_.debug("Button gained focus");
    updateTexture();
    return EventStatus::Consumed;
}

void ButtonBase::updateTexture()
{
    if (isFocused())
    {
        background_.setTexture(focusTexture_);
        return;
    }
    background_.setTexture(getTexture(style_.face.texture.get(state_)));
}

std::unique_ptr<TextButton> TextButton::create(const std::string_view& text)
{
    return std::unique_ptr<TextButton>{new TextButton{text}};
}

TextButton::TextButton(const std::string_view& text)
{
    auto textPtr = gui::Label::create(text, style_.text);
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

IconButton::IconButton(const sf::Texture& texture)
{
    auto icon  = gui::Image::create(texture);
    icon_ = icon.get();

    auto layout = gui::layout::Inset::create(gui::layout::Constraint::Pixels(10.f));
    layout_ = layout.get();

    layout->addChild(std::move(icon));
    addChild(std::move(layout));
}

void IconButton::setIcon(const sf::Texture& icon)
{
    icon_->setTexture(icon);
}

void IconButton::onRender(sf::RenderTexture& renderTexture)
{
    ButtonBase::onRender(renderTexture);
}

void IconButton::onSizeChange()
{
    ButtonBase::onSizeChange();
    layout_->setSize(getSize());
}

void IconButton::onPositionChange()
{
    ButtonBase::onPositionChange();
}

}  // namespace gui
