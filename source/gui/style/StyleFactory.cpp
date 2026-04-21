#include "gui/style/StyleFactory.hpp"

#include <memory>


#include "gui/FontLibrary.hpp"
#include "gui/TextureLibrary.hpp"

namespace gui::style
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
                .color = Stateful<sf::Color>::sameInAllStates(sf::Color::White),
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
                .color = sf::Color::Black
            }
    
        };

        sheet_->window = Window
        {
            .background = sf::Color(200, 200, 200, 255),
            .inactiveBackground = sf::Color(150, 150, 150, 255),
            .header = sf::Color(0, 0, 128, 255),
            .inactiveHeader = sf::Color(140, 140, 140, 255),
            .borderThickness = 1.f,
            .borderColor = sf::Color::Black,
            .titleText =
            {
                .fontName = "ibmVGA",
                .size = 14,
                .color = sf::Color::Black
            }
        };

        sheet_->label = Text
        {
            .fontName = "armata",
            .size = 14,
            .color = sf::Color::Black
        };

        sheet_->progressBar = ProgressBar
        {
            .backgroundColor = sf::Color(255, 255, 255, 255),
            .barColor = sf::Color(0, 0, 128, 255),
            .text =
            {
                .fontName = "armata",
                .size = 14,
                .color = sf::Color::Black
            },
            .borderThickness = 0.f
        };

    return *sheet_;
    }
}

std::unique_ptr<Sheet> StyleFactory::sheet_ = nullptr;

}  // namespace gui::style
