#include "gui/window/Header.hpp"

#include "gui/Alignment.hpp"
#include "gui/Button.hpp"
#include "gui/Label.hpp"
#include "gui/window/Config.hpp"

namespace gui::window
{

Header::Header()
{
    header_shape_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
    header_shape_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());
    header_shape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowHeaderColor());

    // TODO add layout component for button and title text
    auto close_button = std::make_unique<gui::Button>("X");
    close_button->setSize(sf::Vector2f(20.f, window::config::HEADER_HEIGHT));
    close_button->onClick([this]()
    {
        if (closeButtonAction_) closeButtonAction_();
    });
    close_button_ptr_ = close_button.get();
    addChild(std::move(close_button));

    auto title_text = std::make_unique<gui::Label>("");
    title_text->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
    title_text_handle_ = title_text.get();
    title_text_handle_->setSize(getSize());
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
    header_size.y = window::config::HEADER_HEIGHT;
    header_shape_.setSize(header_size);
    title_text_handle_->setSize(header_size);
    close_button_ptr_->setPosition(sf::Vector2f{header_size.x - 20.f, 0.f});
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
