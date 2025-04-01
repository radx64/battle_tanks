#include "gui/window/Header.hpp"

#include "gui/Alignment.hpp"
#include "gui/Button.hpp"
#include "gui/Label.hpp"
#include "gui/window/Config.hpp"
#include "gui/TextureLibrary.hpp"

namespace gui::window
{

Header::Header()
{
    header_shape_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
    header_shape_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());
    header_shape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowHeaderColor());

    // TODO add layout component for button and title text
    auto close_button = gui::TextButton::create("X");
    close_button->onClick([this]()
    {
        if (closeButtonAction_) closeButtonAction_();
    });
    close_button_ptr_ = close_button.get();
    addChild(std::move(close_button));

    auto title_text = gui::Label::create("");
    title_text->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
    title_text_handle_ = title_text.get();
    title_text_handle_->setSize(getSize());
    title_text_handle_->setFontColor(sf::Color::White);
    addChild(std::move(title_text));
}

void Header::closeButtonAction(std::function<void()> closeButtonAction)
{
    closeButtonAction_ = closeButtonAction;
}

void Header::setTitle(const std::string_view& text)
{
    title_text_handle_->setText(text.data());
    // after setText called Label has default bounds
    // so I need to resize it to fill the bar
    title_text_handle_->setSize(getSize());
}

void Header::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(header_shape_);
}

void Header::onSizeChange()
{
    auto header_size = getSize();
    header_shape_.setSize(header_size);
    title_text_handle_->setSize(header_size);
    close_button_ptr_->setSize(sf::Vector2f(config::CLOSE_BUTTON_WIDTH, header_size.y - config::CLOSE_BUTTON_OFFSET.y * 2.f));
    close_button_ptr_->setPosition(sf::Vector2f{header_size.x - config::CLOSE_BUTTON_WIDTH - config::CLOSE_BUTTON_OFFSET.x, config::CLOSE_BUTTON_OFFSET.y});
}

void Header::onPositionChange()
{
    header_shape_.setPosition(getGlobalPosition());

    // TODO: placement of text label should be composed by some layout
    sf::Vector2f tile_text_position {0.0f ,0.0f};
    title_text_handle_->setPosition(tile_text_position);
}

void Header::enable()
{
    header_shape_.setFillColor(BasicStyleSheetFactory::instance().getWindowHeaderColor());
}

void Header::disable()
{
    header_shape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowHeaderColor());
}

}  // namespace gui::window
