#include "engine/gui/style/StyleFactory.hpp"

#include <memory>

#include "engine/gui/FontLibrary.hpp"
#include "engine/gui/TextureLibrary.hpp"

namespace engine::gui::style
{

const Sheet& StyleFactory::instance()
{
    if (sheet_) return *sheet_;
    else
    {
        sheet_ = std::make_unique<Sheet>();

        sheet_->button = Button
        {
            .face = Face
            {
                .color = Stateful<engine::gfx::Color>::sameInAllStates(engine::gfx::Color::White),
                .texture = Stateful<std::string_view>
                {
                    .normal = "button_normal",
                    .hovered = "button_hover",
                    .pressed = "button_pressed"
                }
            },
            .text = Text
            {
                .fontName = "armata",
                .size = 14,
                .color = engine::gfx::Color::Black
            }
    
        };

        sheet_->flatButton = sheet_->button;
        sheet_->flatButton.face.texture = 
        {
            .normal = "flat_button",
            .hovered = "flat_button_hover",
            .pressed = "flat_button_pressed"
        };
        
        sheet_->window = Window
        {
            .background = engine::gfx::Color(200, 200, 200, 255),
            .inactiveBackground = engine::gfx::Color(150, 150, 150, 255),
            .header = engine::gfx::Color(0, 0, 128, 255),
            .inactiveHeader = engine::gfx::Color(140, 140, 140, 255),
            .borderThickness = 1.f,
            .borderColor = engine::gfx::Color::Black,
            .titleText =
            {
                .fontName = "ibmVGA",
                .size = 14,
                .color = engine::gfx::Color::Black
            }
        };

        sheet_->label = Text
        {
            .fontName = "armata",
            .size = 14,
            .color = engine::gfx::Color::Black
        };

        sheet_->progressBar = ProgressBar
        {
            .backgroundColor = engine::gfx::Color(255, 255, 255, 255),
            .barColor = engine::gfx::Color(0, 0, 128, 255),
            .text =
            {
                .fontName = "armata",
                .size = 14,
                .color = engine::gfx::Color::Black
            },
            .borderThickness = 0.f
        };

        sheet_->editBox = EditBox
        {
            .backgroundColor = engine::gfx::Color(255, 255, 255, 255),
            .focusedBackgroundColor = engine::gfx::Color(255, 255, 200, 255),
            .selectionColor = engine::gfx::Color(0, 0, 128, 128),
            .text =
            {
                .fontName = "armata",
                .size = 14,
                .color = engine::gfx::Color::Black
            }
        };

    return *sheet_;
    }
}

std::unique_ptr<Sheet> StyleFactory::sheet_ = nullptr;

}  // namespace engine::gui::style
