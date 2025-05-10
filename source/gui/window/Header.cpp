#include "gui/window/Header.hpp"

#include "gui/Alignment.hpp"
#include "gui/Button.hpp"
#include "gui/Label.hpp"
#include "gui/layout/Horizontal.hpp"
#include "gui/layout/Inset.hpp"
#include "gui/TextureLibrary.hpp"
#include "gui/window/Config.hpp"

namespace gui::window
{

Header::Header()
{
    headerShape_.setOutlineColor(BasicStyleSheetFactory::instance().getOutlineColor());
    headerShape_.setOutlineThickness(BasicStyleSheetFactory::instance().getOutlineThickness());
    headerShape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowHeaderColor());

    auto closeButton = gui::IconButton::create(
        TextureLibrary::instance().get("window_close"));
    closeButton->onClick([this]()
    {
        if(closeButtonAction_) closeButtonAction_();
    });
    closeButtonPtr_ = closeButton.get();

    auto maximizeRestoreButton = gui::IconButton::create(
        TextureLibrary::instance().get("window_maximize"));

    maximizeRestoreButton->onClick([this]()
    {
        if(maximizeRestoreButtonAction_) maximizeRestoreButtonAction_();
    });

    maximizeRestoreButtonPtr_ = maximizeRestoreButton.get();

    auto titleText = gui::Label::create("");
    titleText->setAlignment(gui::Alignment::Left | gui::Alignment::VerticallyCentered);
    titleTextPtr_ = titleText.get();
    titleTextPtr_->setSize(getSize());
    titleTextPtr_->setFontColor(sf::Color::White);

        /*
    +-------------------------------+
    | Inset                         | 
    |  +-------------+-----+-----+  |
    |  | Text        | [ ] |  X  |  |
    |  +-------------+-----+-----+  |
    +-------------------------------+
    */

    auto insetLayout = gui::layout::Inset::create(layout::SizeConstraint::Pixels(config::HEADER_OFFSET));
    auto horizontalLayout = gui::layout::Horizontal::create(0);

    horizontalLayout->addChild(std::move(titleText));
    horizontalLayout->addChild(std::move(maximizeRestoreButton));
    horizontalLayout->addChild(std::move(closeButton));

    horizontalLayout->setColumnSize(0, gui::layout::SizeConstraint::Auto());
    horizontalLayout->setColumnSize(1, gui::layout::SizeConstraint::Pixels(config::HEADER_BUTTON_WIDTH));
    horizontalLayout->setColumnSize(2, gui::layout::SizeConstraint::Pixels(config::HEADER_BUTTON_WIDTH));

    insetLayout->addChild(std::move(horizontalLayout));

    addChild(std::move(insetLayout));
}

void Header::closeButtonAction(std::function<void()> closeButtonAction)
{
    closeButtonAction_ = closeButtonAction;
}

void Header::maximizeRestoreButtonAction(std::function<void()> maximizeRestoreButtonAction)
{
    maximizeRestoreButtonAction_ = maximizeRestoreButtonAction;
}

void Header::setTitle(const std::string_view& text)
{
    titleTextPtr_->setText(text.data());
}

void Header::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(headerShape_);
}

void Header::onSizeChange()
{
    auto header_size = getSize();
    headerShape_.setSize(header_size);

    for (auto& child : children_)
    {
        child->setSize(header_size);
    }
}

void Header::onPositionChange()
{
    headerShape_.setPosition(getGlobalPosition());
}

EventStatus Header::on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    if (isInside(mouseButtonDoublePressedEvent.position))
    {
        if (maximizeRestoreButtonAction_)
        {
            maximizeRestoreButtonAction_();
        }
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

void Header::enable()
{
    headerShape_.setFillColor(BasicStyleSheetFactory::instance().getWindowHeaderColor());
}

void Header::disable()
{
    headerShape_.setFillColor(BasicStyleSheetFactory::instance().getInactiveWindowHeaderColor());
}

void Header::setMaximizeRestoreButtonState(const bool& state)
{
    if (state)
    {
        maximizeRestoreButtonPtr_->setIcon(
            TextureLibrary::instance().get("window_restore"));
    }
    else
    {
        maximizeRestoreButtonPtr_->setIcon(
            TextureLibrary::instance().get("window_maximize"));
    }
}

}  // namespace gui::window
