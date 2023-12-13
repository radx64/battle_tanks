#include "gui/Button.hpp"

#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{
Button::Button(const std::string_view& text)
: isButtonHoldDown_(false)
{
    auto style = BasicStyleSheetFactory::instance();    
    background_.setFillColor(style.getWindowColor());
    background_.setOutlineColor(style.getOutlineColor());
    background_.setOutlineThickness(style.getOutlineThickness()); 
    background_.setPosition(getGlobalPosition());
    background_.setSize(Component::getSize());
    auto text_ptr = std::make_unique <gui::Label>(text);
    text_ = text_ptr.get();
    this->addChild(std::move(text_ptr));
}

Button::~Button() = default;

void Button::setSize(const sf::Vector2f& size)
{
    background_.setSize(size);
    Component::setSize(size);
    
    auto button_size = getSize();
    auto text_size = text_->getSize();
    text_->setPosition(sf::Vector2f{(button_size.x - text_size.x) / 2.f, (button_size.y - text_size.y) / 2.f}, alignment_);
}

void Button::setPosition(const sf::Vector2f& position, const Alignment alignment)
{
    Component::setPosition(position, alignment);
    UNUSED(position);
    UNUSED(alignment);
    text_->setPosition(getSize() / 2.f, gui::Alignment::CENTERED);
    background_.setPosition(Component::getGlobalPosition());
}

void Button::onParentPositionChange(const sf::Vector2f& parent_position)
{
    UNUSED(parent_position);
    text_->setPosition(getSize() / 2.f, gui::Alignment::CENTERED);
    background_.setPosition(Component::getGlobalPosition());
}

void Button::setText(const sf::String& text)
{
    text_->setText(text);
}

void Button::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
}

void Button::onClick(std::function<void()> onClickCallback)
{
    on_click_ = onClickCallback;
}

EventStatus Button::on(const event::MouseEntered&)
{
    background_.setFillColor(sf::Color(255,0,0,255));
    return gui::EventStatus::Consumed;
}
EventStatus Button::on(const event::MouseLeft&)
{
    background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
    return gui::EventStatus::Consumed;
}

EventStatus Button::on(const event::MouseButtonPressed& mousePressedEvent)
{
    if (not is_visible_) return gui::EventStatus::NotConsumed;
    
    auto mousePosition = sf::Vector2f{mousePressedEvent.position.x, mousePressedEvent.position.y};
    bool isLeftClicked = mousePressedEvent.button == gui::event::MouseButton::Left;

    if (isLeftClicked and background_.getGlobalBounds().contains(mousePosition))
    {
        background_.setFillColor(sf::Color(0,255,0,255));
        if (not isButtonHoldDown_) 
        {
            isButtonHoldDown_ = true;
            if (on_click_) on_click_();  
        }
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus Button::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (not isButtonHoldDown_) return gui::EventStatus::NotConsumed;
    bool isLeftReleased = mouseButtonReleasedEvent.button == gui::event::MouseButton::Left;

    if (isLeftReleased)
    {
        isButtonHoldDown_ = false;
        background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

}  // namespace gui