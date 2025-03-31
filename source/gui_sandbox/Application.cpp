#include "Application.hpp"

#include <chrono>
#include <fmt/format.h>

#include "Config.hpp"

#include "engine/Context.hpp"
#include "engine/Logger.hpp"
#include "engine/Timer.hpp"
#include "engine/TimerService.hpp"

#include "gui/Debug.hpp"
#include "gui/Button.hpp"
#include "gui/EditBox.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/Layout.hpp"
#include "gui/Window.hpp"
#include "gui/ProgressBar.hpp"
#include "gui/FramedSprite.hpp"

#include "gui/TextureLibrary.hpp"

using namespace std::literals;

namespace gui_sandbox
{
Application::Application()
: engine::Application{"GUI sandbox", "Main application"}
, windowManager_{sf::Vector2f{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}}
, mouseController_{&windowManager_, window_, window_.getDefaultView()}
, keyboardController_{&windowManager_}
, textEnteredController_{&windowManager_}
, timer_{5s, [this](){logger_.debug("5 seconds heartbeat ❤️ \n");}}
{
    auto& timerService = engine::Context::getTimerService();
    timerService.start(&timer_, engine::TimerType::Repeating);
}

void Application::onInit()
{
    gui::FontLibrary::initialize();
    gui::TextureLibrary::init();

    window_.setFramerateLimit(60);
    window_.setVerticalSyncEnabled(true);

    generateBackground();

    mouseHandler_.subscribe(&mouseController_);
    keyboardHandler_.subscribe(&keyboardController_);
    keyboardHandler_.subscribe(&textEnteredController_);

    auto quitButton = gui::TextButton::create("Quit");
    quitButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 50.f));
    quitButton->setSize(sf::Vector2f(250.f, 30.f));
    quitButton->onClick([this](){logger_.info("Quitting..."); Application::close();});
    windowManager_.mainWindow().addChild(std::move(quitButton));

    auto guiDebug = gui::TextButton::create("GUI DEBUG");
    guiDebug->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, Config::WINDOW_HEIGHT - 50.f));
    guiDebug->setSize(sf::Vector2f(250.f, 30.f));
    guiDebug->onClick([](){gui::debug::toggle();});
    windowManager_.mainWindow().addChild(std::move(guiDebug));

    auto createEmptyWindowButton = gui::TextButton::create("Empty Window");
    createEmptyWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 100.f));
    createEmptyWindowButton->setSize(sf::Vector2f(250.f, 30.f));

    createEmptyWindowButton->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("Nothing here!");

        windowManager_.addWindow(std::move(window));
    });

    windowManager_.mainWindow().addChild(std::move(createEmptyWindowButton));

    auto createSimpleWindowButton = gui::TextButton::create("Simple Window");
    createSimpleWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 150.f));
    createSimpleWindowButton->setSize(sf::Vector2f(250.f, 30.f));

    createSimpleWindowButton->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        auto simpleButton = gui::TextButton::create("THIS IS BUTTON!");
        window->addChild(std::move(simpleButton));

        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("So simple!");

        windowManager_.addWindow(std::move(window));

    });

    windowManager_.mainWindow().addChild(std::move(createSimpleWindowButton));

    auto createProgressWindowButton = gui::TextButton::create("ProgressBar Window");
    createProgressWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 200.f));
    createProgressWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createProgressWindowButton->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("I see some progress :D");

        auto verticalLayout = gui::VerticalLayout::create();
        auto minusButton = gui::TextButton::create("-");
        auto progressBar = gui::ProgressBar::create();
        auto plusButton = gui::TextButton::create("+");


        progressBar->setRange(0.f, 25.f);

        minusButton->onClick([progressBarPtr = progressBar.get()]{
            progressBarPtr->setValue(progressBarPtr->getValue()-1.f);
        });

        plusButton->onClick([progressBarPtr = progressBar.get()]{
            progressBarPtr->setValue(progressBarPtr->getValue()+1.f);
        });

        verticalLayout->addChild(std::move(minusButton));
        verticalLayout->addChild(std::move(progressBar));
        verticalLayout->addChild(std::move(plusButton));

        window->addChild(std::move(verticalLayout));

        windowManager_.addWindow(std::move(window));

    });

    windowManager_.mainWindow().addChild(std::move(createProgressWindowButton));

    auto createEditboxWindowButton = gui::TextButton::create("Edit Box Window");
    createEditboxWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 250.f));
    createEditboxWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createEditboxWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("I can now read text from keyboard :D");

        auto verticalLayout = gui::VerticalLayout::create();
        auto button = gui::TextButton::create("CLICK HERE");
        auto editBox = gui::EditBox::create();
        auto editBox2 = gui::EditBox::create();

        button->onClick([editBoxPtr = editBox.get(), editBox2Ptr = editBox2.get(), button_ptr = button.get()]{
            button_ptr->setText(editBoxPtr->getText() + " | " + editBox2Ptr->getText());
        });

        verticalLayout->addChild(std::move(button));
        verticalLayout->addChild(std::move(editBox));
        verticalLayout->addChild(std::move(editBox2));

        window->addChild(std::move(verticalLayout));

        windowManager_.addWindow(std::move(window));

    });

    windowManager_.mainWindow().addChild(std::move(createEditboxWindowButton));

    auto createLayoutWindowButton = gui::TextButton::create("Layout Window");
    createLayoutWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 300.f));
    createLayoutWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createLayoutWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();
        auto horizontalLayout = gui::HorizontalLayout::create();
        auto helloButton = gui::TextButton::create("HELLO");
        auto worldButton = gui::TextButton::create("WORLD");

        auto horizontalLayout2 = gui::HorizontalLayout::create();
        auto somethingButton = gui::TextButton::create("SOMETHING");
        auto cookingButton = gui::TextButton::create("IS COOKING");
        auto hardButton = gui::TextButton::create("HARD");

        helloButton->onClick([this](){logger_.info("Hello?");});
        worldButton->onClick([this](){logger_.info("Is it me you looking for?");});

        horizontalLayout->addChild(std::move(helloButton));
        horizontalLayout->addChild(std::move(worldButton));

        horizontalLayout2->addChild(std::move(somethingButton));

        auto verticalLayout2 = gui::VerticalLayout::create();
        verticalLayout2->addChild(std::move(cookingButton));
        verticalLayout2->addChild(std::move(hardButton));

        horizontalLayout2->addChild(std::move(verticalLayout2));

        auto verticalLayout = gui::VerticalLayout::create();

        verticalLayout->addChild(std::move(horizontalLayout));
        verticalLayout->addChild(std::move(horizontalLayout2));

        window->addChild(std::move(verticalLayout));

        window->setSize(sf::Vector2f(700.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("Oh my gosh");

        windowManager_.addWindow(std::move(window));
    });

    windowManager_.mainWindow().addChild(std::move(createLayoutWindowButton));

    auto createFocusTestWindowButton = gui::TextButton::create("Focus Test Window");
    createFocusTestWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 350.f));
    createFocusTestWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createFocusTestWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();

        auto hl1 = gui::HorizontalLayout::create();
        auto edit1 = gui::EditBox::create();
        edit1->setAlignment(gui::Alignment::Left);
        auto button1 = gui::TextButton::create("Button1");
        hl1->addChild(std::move(edit1));
        hl1->addChild(std::move(button1));

        auto hl2 = gui::HorizontalLayout::create();
        auto edit2 = gui::EditBox::create();
        edit2->setAlignment(gui::Alignment::HorizontallyCentered);
        auto button2 = gui::TextButton::create("Button2");
        hl2->addChild(std::move(edit2));
        hl2->addChild(std::move(button2));

        auto hl3 = gui::HorizontalLayout::create();
        auto edit3 = gui::EditBox::create();
        edit3->setAlignment(gui::Alignment::Right);
        auto button3 = gui::TextButton::create("Button3");
        hl3->addChild(std::move(edit3));
        hl3->addChild(std::move(button3));

        auto hl4 = gui::HorizontalLayout::create();
        auto edit4 = gui::EditBox::create();
        auto button4 = gui::TextButton::create("Button4");
        hl4->addChild(std::move(edit4));
        hl4->addChild(std::move(button4));

        auto vl = gui::VerticalLayout::create();
        vl->addChild(std::move(hl1));
        vl->addChild(std::move(hl2));
        vl->addChild(std::move(hl3));
        vl->addChild(std::move(hl4));

        window->addChild(std::move(vl));

        window->setSize(sf::Vector2f(700.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("I need to focus");

        windowManager_.addWindow(std::move(window));
    });

    windowManager_.mainWindow().addChild(std::move(createFocusTestWindowButton));

    auto createGridLayoutWindowButton = gui::TextButton::create("Grid Layout Window");
    createGridLayoutWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 400.f));
    createGridLayoutWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createGridLayoutWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();

        auto gridLayout = gui::GridLayout::create(2, 3);

        auto verticalLayout = gui::VerticalLayout::create();
        auto horizontalLayout = gui::HorizontalLayout::create();

        auto positionBox = gui::VerticalLayout::create();
        auto positionLabel = gui::Label::create("Position:");
        positionLabel->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        auto positionValue = gui::EditBox::create();
        positionValue->setText("0");
        positionValue->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        auto* positionValuePtr = positionValue.get();

        auto ratioLabel = gui::Label::create("Ratio:");
        ratioLabel->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        auto ratioValue = gui::EditBox::create();
        ratioValue->setText("0.20");
        ratioValue->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        auto* ratioValuePtr = ratioValue.get();

        positionBox->addChild(std::move(positionLabel));
        positionBox->addChild(std::move(positionValue));
        positionBox->addChild(std::move(ratioLabel));
        positionBox->addChild(std::move(ratioValue));

        auto gridStatusLabel = gui::Label::create("");
        gridStatusLabel->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);

        auto updateGridStatusLabel = [gridLayoutPtr = gridLayout.get(), gridStatusLabelPtr = gridStatusLabel.get()]()
        {
            gridStatusLabelPtr->setText(fmt::format("Grid size: {}x{}", gridLayoutPtr->getWidth(), 
                gridLayoutPtr->getHeight()));
        };

        positionBox->addChild(std::move(gridStatusLabel));

        updateGridStatusLabel();

        auto addNewColumnButton = gui::TextButton::create("Add Column");
        auto removeLastColumnButton = gui::TextButton::create("Remove Column");
        auto addNewRowButton = gui::TextButton::create("Add Row");
        auto removeLastRowButton = gui::TextButton::create("Remove Row");

        auto getPositionValue = [positionValuePtr]()
        {
            return std::stoi(positionValuePtr->getText());
        }; 

        auto getRatioValue = [ratioValuePtr]() -> std::optional<float>
        {
            auto text = ratioValuePtr->getText();

            if (text.size() > 0)
            {
                return std::stof(text);
            }
            else
            {
                return std::nullopt;
            }
        };

        addNewColumnButton->onClick([this, gridLayoutPtr = gridLayout.get(), updateGridStatusLabel, getPositionValue, getRatioValue]{
            logger_.info("Adding new column to grid layout");
            
            auto position = 0;
            std::optional<float> ratio = 0.0f;
            try
            {
                position = getPositionValue();
                ratio = getRatioValue();
            } 
            catch (const std::invalid_argument& e)
            {
                logger_.error(fmt::format("Invalid argument: {}", e.what()));
                return;
            }

            if(not gridLayoutPtr->addColumn(position)) return;

            if (ratio.has_value()) gridLayoutPtr->setColumnSize(position, ratio.value());

            updateGridStatusLabel();

            for (size_t i = 0; i < gridLayoutPtr->getHeight(); ++i)
            {
                auto button = gui::TextButton::create("NEW BUTTON " + std::to_string(i));
                gridLayoutPtr->addChild(std::move(button));
            }
        });

        removeLastColumnButton->onClick([this, gridLayoutPtr = gridLayout.get(), updateGridStatusLabel, getPositionValue]{
            logger_.info("Removing column from grid layout");

            auto position  = 0;
            try
            {
                position = getPositionValue();
            } 
            catch (const std::invalid_argument& e)
            {
                logger_.error(fmt::format("Invalid argument: {}", e.what()));
                return;
            }

            if(not gridLayoutPtr->removeColumn(position)) return;
            updateGridStatusLabel();
        });

        addNewRowButton->onClick([this, gridLayoutPtr = gridLayout.get(), updateGridStatusLabel, getPositionValue, getRatioValue]{
            logger_.info("Adding new row to grid layout");

            auto position  = 0;
            std::optional<float> ratio = 0.0f;
            try
            {
                position = getPositionValue();
                ratio = getRatioValue();
            } 
            catch (const std::invalid_argument& e)
            {
                logger_.error(fmt::format("Invalid argument: {}", e.what()));
                return;
            }

            if(not gridLayoutPtr->addRow(position)) return;
            if (ratio.has_value()) gridLayoutPtr->setRowSize(position, ratio.value());
            updateGridStatusLabel();

            for (size_t i = 0; i < gridLayoutPtr->getWidth(); ++i)
            {
                auto button = gui::TextButton::create("NEW BUTTON " + std::to_string(i));
                gridLayoutPtr->addChild(std::move(button));
            }
        });

        removeLastRowButton->onClick([this, gridLayoutPtr = gridLayout.get(), updateGridStatusLabel, getPositionValue]{
            logger_.info("Removing row from grid layout");

            auto position  = 0;
            try
            {
                position = getPositionValue();
            } 
            catch (const std::invalid_argument& e)
            {
                logger_.error(fmt::format("Invalid argument: {}", e.what()));
                return;
            }

            if(not gridLayoutPtr->removeRow(position)) return;
            updateGridStatusLabel();
        });


        horizontalLayout->addChild(std::move(positionBox));
        horizontalLayout->addChild(std::move(addNewColumnButton));
        horizontalLayout->addChild(std::move(removeLastColumnButton));
        horizontalLayout->addChild(std::move(addNewRowButton));
        horizontalLayout->addChild(std::move(removeLastRowButton));

        verticalLayout->addChild(std::move(horizontalLayout));

        auto helloButton = gui::TextButton::create("HELLO");
        auto worldButton = gui::TextButton::create("WORLD");
        auto somethingButton = gui::TextButton::create("SOMETHING");
        auto cookingButton = gui::TextButton::create("IS COOKING");
        auto hardButton = gui::TextButton::create("HARD");

        helloButton->onClick([this](){logger_.info("Hello?");});
        worldButton->onClick([this](){logger_.info("Is it me you looking for?");});

        gridLayout->addChild(std::move(helloButton));
        gridLayout->addChild(std::move(worldButton));
        gridLayout->addChild(std::move(somethingButton));
        gridLayout->addChild(std::move(cookingButton));
        gridLayout->addChild(std::move(hardButton));


        verticalLayout->addChild(std::move(gridLayout));

        window->addChild(std::move(verticalLayout));

        window->setSize(sf::Vector2f(700.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("The Grid, a digital frontier...");

        windowManager_.addWindow(std::move(window));

    });

    windowManager_.mainWindow().addChild(std::move(createGridLayoutWindowButton));

    auto leftLabel = gui::Label::create("Left aligned label");
    leftLabel->setPosition({100.f, 20.f});
    leftLabel->setSize({400.f, 40.f});
    leftLabel->setAlignment(gui::Alignment::Left);

    auto hcenterLabel = gui::Label::create("Horizontally centered label");
    hcenterLabel->setPosition({100.f, 80.f});
    hcenterLabel->setSize({400.f, 40.f});
    hcenterLabel->setAlignment(gui::Alignment::HorizontallyCentered);

    auto rightLabel = gui::Label::create("Right aligned label");
    rightLabel->setPosition({100.f, 140.f});
    rightLabel->setSize({400.f, 40.f});
    rightLabel->setAlignment(gui::Alignment::Right);

    windowManager_.mainWindow().addChild(std::move(leftLabel));
    windowManager_.mainWindow().addChild(std::move(hcenterLabel));
    windowManager_.mainWindow().addChild(std::move(rightLabel));

    auto topLabel = gui::Label::create("Top aligned label");
    topLabel->setPosition({700.f, 20.f});
    topLabel->setSize({400.f, 40.f});
    topLabel->setAlignment(gui::Alignment::Top);

    auto vcenterLabel = gui::Label::create("Vertically centered label");
    vcenterLabel->setPosition({700.f, 80.f});
    vcenterLabel->setSize({400.f, 40.f});
    vcenterLabel->setAlignment(gui::Alignment::VerticallyCentered);

    auto bottomLabel = gui::Label::create("Bottom aligned label");
    bottomLabel->setPosition({700.f, 140.f});
    bottomLabel->setSize({400.f, 40.f});
    bottomLabel->setAlignment(gui::Alignment::Bottom);

    windowManager_.mainWindow().addChild(std::move(topLabel));
    windowManager_.mainWindow().addChild(std::move(vcenterLabel));
    windowManager_.mainWindow().addChild(std::move(bottomLabel));

    auto multilineTopAlignedLabel = gui::Label::create("Top multiline\nlabel is here\nand a bit there");
    multilineTopAlignedLabel->setPosition({100.f, 300.f});
    multilineTopAlignedLabel->setSize({400.f, 80.f});
    multilineTopAlignedLabel->setAlignment(gui::Alignment::Top);

    auto multilineCenterAlignedLabel = gui::Label::create("Centered multiline\nlabel is here\nand a bit there");
    multilineCenterAlignedLabel->setPosition({100.f, 400.f});
    multilineCenterAlignedLabel->setSize({400.f, 80.f});
    multilineCenterAlignedLabel->setAlignment(gui::Alignment::VerticallyCentered);

    auto multilineBottomAlignedLabel = gui::Label::create("Bottom multiline\nlabel is here\nand a bit there");
    multilineBottomAlignedLabel->setPosition({100.f, 500.f});
    multilineBottomAlignedLabel->setSize({400.f, 80.f});
    multilineBottomAlignedLabel->setAlignment(gui::Alignment::Bottom);

    windowManager_.mainWindow().addChild(std::move(multilineTopAlignedLabel));
    windowManager_.mainWindow().addChild(std::move(multilineCenterAlignedLabel));
    windowManager_.mainWindow().addChild(std::move(multilineBottomAlignedLabel));

    auto layout = gui::FramedSprite::LayoutConfig
    {
        .cornerSizes = 
        {
            .topLeft        = {20.f, 20.f},
            .bottomRight    = {20.f, 20.f}
        },
        .uvs = 
        {
            .topLeft        = {0.0f, 0.0f, 1.0f, 1.0f},
            .topRight       = {2.0f, 0.0f, 1.0f, 1.0f},
            .bottomLeft     = {0.0f, 2.0f, 1.0f, 1.0f},
            .bottomRight    = {2.0f, 2.0f, 1.0f, 1.0f},
        }
    };

    framedSpriteTest_ = std::make_unique<gui::FramedSprite>(layout);
    framedSpriteTest_->setTexture(gui::TextureLibrary::instance().get("framedSpriteTest"));
    framedSpriteTest_->setPosition(200.f, 400.f);
    framedSpriteTest_->setSize({300.f, 300.f});

    layout = gui::FramedSprite::LayoutConfig
    {
        .cornerSizes = 
        {
            .topLeft        = {20.f, 20.f},
            .bottomRight    = {20.f, 20.f}
        },
        .uvs = 
        {
            .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
            .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
            .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
            .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
        }
    };

    framedSpriteTest2_ = std::make_unique<gui::FramedSprite>(layout);
    framedSpriteTest2_->setTexture(gui::TextureLibrary::instance().get("button_focus"));
    framedSpriteTest2_->setPosition(600.f, 400.f);
    framedSpriteTest2_->setSize({300.f, 300.f});

}

void Application::onClose()
{
    gui::FontLibrary::destroy();
}

void Application::onEvent(const sf::Event& event)
{
    (void) event;
}

void Application::onUpdate(float timeStep)
{
    (void) timeStep;

    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(timerService_.getCurrentTime().time_since_epoch()).count();
    float scaleX = std::sin(time / 500.f) * 0.5f + 1.f; // scale oscillates between 0.5 and 1.5
    float scaleY = std::cos(time / 500.f) * 0.5f + 1.f;
    framedSpriteTest_->setSize({300.f * scaleX, 300.f * scaleY});
    framedSpriteTest2_->setSize({300.f * scaleY, 300.f * scaleX});
}

void Application::generateBackground()
{
    backgroundTexture_.create(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    backgroundSprite_.setTexture(backgroundTexture_.getTexture());

    const auto EDGE_OFFSET = 20.f;
    const auto LINE_THICKNESS = 10.f;
    const auto X_STEPS = 20;
    const auto Y_STEPS = 10;

    sf::RectangleShape outerBox{};
    outerBox.setPosition(EDGE_OFFSET, EDGE_OFFSET);
    outerBox.setSize(sf::Vector2f{Config::WINDOW_WIDTH - 2 * EDGE_OFFSET, Config::WINDOW_HEIGHT -  2*EDGE_OFFSET});
    outerBox.setOutlineThickness(LINE_THICKNESS);
    outerBox.setFillColor(sf::Color::Transparent);
    outerBox.setOutlineColor(sf::Color(255,255,255,240));
    backgroundTexture_.draw(outerBox);

    for (auto x = 1; x < X_STEPS; ++x)
    {
        auto xCoord = x * ((Config::WINDOW_WIDTH - 2*EDGE_OFFSET) / X_STEPS) + EDGE_OFFSET;
        sf::RectangleShape line{};
        line.setPosition(xCoord, EDGE_OFFSET);
        line.setSize(sf::Vector2f{LINE_THICKNESS/4.f, Config::WINDOW_HEIGHT -  2*EDGE_OFFSET});
        line.setFillColor(sf::Color(255,255,255,200));
        backgroundTexture_.draw(line);
    }

    for (auto y = 1; y < Y_STEPS; ++y)
    {
        auto yCoord = y * ((Config::WINDOW_HEIGHT- 2*EDGE_OFFSET) / Y_STEPS) + EDGE_OFFSET;
        sf::RectangleShape line{};
        line.setPosition(EDGE_OFFSET, yCoord);
        line.setSize(sf::Vector2f{Config::WINDOW_WIDTH -  2*EDGE_OFFSET, LINE_THICKNESS/4.f});
        line.setFillColor(sf::Color(255,255,255,200));
        backgroundTexture_.draw(line);
    }
    backgroundTexture_.display();
}

void Application::onRender()
{
    window_.clear(sf::Color(20, 110, 158));
    window_.draw(backgroundSprite_);
    window_.draw(*framedSpriteTest_);
    window_.draw(*framedSpriteTest2_);
    windowManager_.render(window_);
}

}  // namespace gui_sandbox
