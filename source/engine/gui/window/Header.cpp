#include "engine/gui/window/Header.hpp"

#include "engine/gui/Alignment.hpp"
#include "engine/gui/Button.hpp"
#include "engine/gui/Label.hpp"
#include "engine/gui/layout/Horizontal.hpp"
#include "engine/gui/layout/Inset.hpp"
#include "engine/gui/TextureLibrary.hpp"
#include "engine/gui/window/Config.hpp"

#include "engine/gui/style/StyleFactory.hpp"

namespace engine::gui::window
{

Header::Header()
: style_{style::StyleFactory::instance().window}
{

    headerShape_.setOutlineColor(style_.borderColor);
    headerShape_.setOutlineThickness(style_.borderThickness);
    headerShape_.setFillColor(style_.inactiveHeader);

    auto closeButton = engine::gui::IconButton::create(
        TextureLibrary::instance().get("window_close"));
    closeButton->onClick([this]()
    {
        if(closeButtonAction_) closeButtonAction_();
    });
    closeButton->disableFocus();
    closeButtonPtr_ = closeButton.get();

    auto maximizeRestoreButton = engine::gui::IconButton::create(
        TextureLibrary::instance().get("window_maximize"));

    maximizeRestoreButton->onClick([this]()
    {
        if(maximizeRestoreButtonAction_) maximizeRestoreButtonAction_();
    });
    maximizeRestoreButton->disableFocus();
    maximizeRestoreButtonPtr_ = maximizeRestoreButton.get();

    auto titleText = engine::gui::Label::create("");
    titleText->setAlignment(engine::gui::Alignment::Left | engine::gui::Alignment::VerticallyCentered);
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

    auto insetLayout = engine::gui::layout::Inset::create(layout::Constraint::Pixels(config::HEADER_OFFSET));
    auto horizontalLayout = engine::gui::layout::Horizontal::create(0);

    horizontalLayout->addChild(std::move(titleText));
    horizontalLayout->addChild(std::move(maximizeRestoreButton));
    horizontalLayout->addChild(std::move(closeButton));

    horizontalLayout->setColumnSize(0, engine::gui::layout::Constraint::Auto());
    horizontalLayout->setColumnSize(1, engine::gui::layout::Constraint::Pixels(config::HEADER_BUTTON_WIDTH));
    horizontalLayout->setColumnSize(2, engine::gui::layout::Constraint::Pixels(config::HEADER_BUTTON_WIDTH));

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
    headerShape_.setFillColor(style_.header);
}

void Header::disable()
{
    headerShape_.setFillColor(style_.inactiveHeader);
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

}  // namespace engine::gui::window
