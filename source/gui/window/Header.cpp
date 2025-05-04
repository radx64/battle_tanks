#include "gui/window/Header.hpp"

#include "gui/Alignment.hpp"
#include "gui/Button.hpp"
#include "gui/Label.hpp"
#include "gui/layout/Grid.hpp"
#include "gui/TextureLibrary.hpp"
#include "gui/window/Config.hpp"

namespace gui::window
{

Header::Header()
{
    header_shape_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
    header_shape_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());
    header_shape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowHeaderColor());

    auto close_button = gui::TextButton::create("X");
    close_button->onClick([this]()
    {
        if(closeButtonAction_) closeButtonAction_();
    });
    close_button_ptr_ = close_button.get();

    auto title_text = gui::Label::create("");
    title_text->setAlignment(gui::Alignment::Left | gui::Alignment::VerticallyCentered);
    title_text_handle_ = title_text.get();
    title_text_handle_->setSize(getSize());
    title_text_handle_->setFontColor(sf::Color::White);

    auto layout = gui::layout::Grid::create(4,3);

    /*
     // vertical border
    +--+--------+----+--+
    |  |        |    |  | // horizontal border
    +--+--------+----+--+
    |  | Text   | X  |  |
    +--+--------+----+--+
    |  |        |    |  | // horizontal border
    +--+--------+----+--+ 
                      \\ vertical border
    */

    layout->setColumnSize(0, gui::layout::SizeConstraint::Pixels(config::CLOSE_BUTTON_OFFSET.x));
    layout->setColumnSize(1, gui::layout::SizeConstraint::Auto());
    layout->setColumnSize(2, gui::layout::SizeConstraint::Pixels(config::CLOSE_BUTTON_WIDTH));
    layout->setColumnSize(3, gui::layout::SizeConstraint::Pixels(config::CLOSE_BUTTON_OFFSET.x));

    layout->setRowSize(0, gui::layout::SizeConstraint::Pixels(config::CLOSE_BUTTON_OFFSET.y));
    layout->setRowSize(1, gui::layout::SizeConstraint::Auto());
    layout->setRowSize(2, gui::layout::SizeConstraint::Pixels(config::CLOSE_BUTTON_OFFSET.y));

    layout->setElementAt(1, 1, std::move(title_text));
    layout->setElementAt(2, 1, std::move(close_button));
    addChild(std::move(layout));
}

void Header::closeButtonAction(std::function<void()> closeButtonAction)
{
    closeButtonAction_ = closeButtonAction;
}

void Header::setTitle(const std::string_view& text)
{
    title_text_handle_->setText(text.data());
}

void Header::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(header_shape_);
}

void Header::onSizeChange()
{
    auto header_size = getSize();
    header_shape_.setSize(header_size);

    for (auto& child : children_)
    {
        child->setSize(header_size);
    }
}

void Header::onPositionChange()
{
    header_shape_.setPosition(getGlobalPosition());
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
