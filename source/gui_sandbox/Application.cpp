#include "Application.hpp"

#include <chrono>
#include <fmt/format.h>

#include "Config.hpp"

#include "engine/Context.hpp"
#include "engine/Logger.hpp"
#include "engine/Timer.hpp"
#include "engine/TimerService.hpp"

#include "gui/Button.hpp"
#include "gui/Checkbox.hpp"
#include "gui/Debug.hpp"
#include "gui/EditBox.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/FramedSprite.hpp"
#include "gui/Label.hpp"
#include "gui/layout/Grid.hpp"
#include "gui/layout/Horizontal.hpp"
#include "gui/layout/Inset.hpp"
#include "gui/layout/Vertical.hpp"
#include "gui/ProgressBar.hpp"
#include "gui/RadioButton.hpp"
#include "gui/RadioButtonGroup.hpp"
#include "gui/Slider.hpp"
#include "gui/TextureLibrary.hpp"
#include "gui/Window.hpp"

using namespace std::literals;

namespace gui_sandbox
{
Application::Application()
: engine::Application{"GUI sandbox", "Main application"}
, windowManager_{sf::Vector2f{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}}
, mouseController_{&windowManager_, window_, window_.getDefaultView()}
, keyboardController_{&windowManager_}
, textEnteredController_{&windowManager_}
, timer_{5s, [this](){logger_.info("5 seconds heartbeat ❤️ \n");}}
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
        window->setSize(sf::Vector2f(300.0f, 300.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2 - 150.f, 400.0f));
        window->setTitle("Nothing here!");

        windowManager_.addWindow(std::move(window));
    });

    windowManager_.mainWindow().addChild(std::move(createEmptyWindowButton));

    auto createSimpleWindowButton = gui::TextButton::create("Simple Window");
    createSimpleWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 150.f));
    createSimpleWindowButton->setSize(sf::Vector2f(250.f, 30.f));

    createSimpleWindowButton->onClick([this](){
        auto window = std::make_unique<gui::Window>();

        auto insetLayout = gui::layout::Inset::create(gui::layout::SizeConstraint::Percent(75.f));
        auto simpleButton = gui::TextButton::create("THIS IS BUTTON!");
        insetLayout->addChild(std::move(simpleButton));
        window->addChild(std::move(insetLayout));

        window->setSize(sf::Vector2f(300.0f, 300.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2 - 150.f, 400.0f));
        window->setTitle("So simple!");

        windowManager_.addWindow(std::move(window));
    });

    windowManager_.mainWindow().addChild(std::move(createSimpleWindowButton));

    auto createProgressWindowButton = gui::TextButton::create("ProgressBar Window");
    createProgressWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 200.f));
    createProgressWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createProgressWindowButton->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(300.0f, 300.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2 - 150.f, 400.0f));
        window->setTitle("I see some progress :D");

        auto verticalLayout = gui::layout::Vertical::create();
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
        window->setSize(sf::Vector2f(300.0f, 300.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2 - 150.f, 400.0f));
        window->setTitle("I can now read text from keyboard :D");

        auto verticalLayout = gui::layout::Vertical::create();
        auto button = gui::TextButton::create("CLICK HERE");
        auto editBox = gui::EditBox::create();
        editBox->setAlignment(gui::Alignment::VerticallyCentered);
        auto editBox2 = gui::EditBox::create();
        editBox2->setAlignment(gui::Alignment::VerticallyCentered);
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

    auto createLayoutWindowButton = gui::TextButton::create("Simple Layout Window");
    createLayoutWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 300.f));
    createLayoutWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createLayoutWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();

        auto horizontalLayout = gui::layout::Horizontal::create();
        auto helloButton = gui::TextButton::create("HELLO");
        auto worldButton = gui::TextButton::create("WORLD");
        helloButton->onClick([this](){logger_.info("Hello?");});
        worldButton->onClick([this](){logger_.info("Is it me you looking for?");});
        horizontalLayout->addChild(std::move(helloButton));
        horizontalLayout->addChild(std::move(worldButton));

        
        auto horizontalLayout2 = gui::layout::Horizontal::create();
        auto checkbox = gui::Checkbox::create(false);
        checkbox->onStateChange([this](bool state)
        {
            logger_.info(fmt::format("Checkbox 1 state changed, new state: {}", state));
        });
        auto checkbox_text = gui::Label::create("Checkbox 1");
        checkbox_text->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        horizontalLayout2->addChild(std::move(checkbox));
        horizontalLayout2->addChild(std::move(checkbox_text));

        auto horizontalLayout3 = gui::layout::Horizontal::create();
        auto checkbox2 = gui::Checkbox::create(false);
        checkbox2->onStateChange([this](bool state)
        {
            logger_.info(fmt::format("Checkbox 2 state changed, new state: {}", state));
        });

        auto checkbox_text2 = gui::Label::create("Checkbox 2");
        checkbox_text2->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        horizontalLayout3->addChild(std::move(checkbox2));
        horizontalLayout3->addChild(std::move(checkbox_text2));
        
        auto horizontalLayout4 = gui::layout::Horizontal::create();
        auto radiobutton = gui::RadioButton::create(true);
        radiobutton->setState(false);
        auto radiobutton_text = gui::Label::create("RadioButton 1");
        radiobutton_text->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        
        auto horizontalLayout5 = gui::layout::Horizontal::create();
        auto radiobutton2 = gui::RadioButton::create(true);
        radiobutton2->setState(false);
        auto radiobutton_text2 = gui::Label::create("RadioButton 2");
        radiobutton_text2->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        
        auto horizontalLayout6 = gui::layout::Horizontal::create();
        auto radiobutton3 = gui::RadioButton::create(true);
        radiobutton3->setState(false);
        auto radiobutton_text3 = gui::Label::create("RadioButton 3");
        radiobutton_text3->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);

        auto radioButtonGroup = gui::RadioButtonGroup::create();
        radioButtonGroup->add(radiobutton.get());
        radioButtonGroup->add(radiobutton2.get());
        radioButtonGroup->add(radiobutton3.get());

        radiobutton->onStateChange([this](bool state)
        {
            logger_.info(fmt::format("Radiobutton 1 state changed, new state: {}", state));
        });

        radiobutton2->onStateChange([this](bool state)
        {
            logger_.info(fmt::format("Radiobutton 2 state changed, new state: {}", state));
        });

        radiobutton3->onStateChange([this](bool state)
        {
            logger_.info(fmt::format("Radiobutton 3 state changed, new state: {}", state));
        });

        horizontalLayout4->addChild(std::move(radiobutton));
        horizontalLayout4->addChild(std::move(radiobutton_text));

        horizontalLayout5->addChild(std::move(radiobutton2));
        horizontalLayout5->addChild(std::move(radiobutton_text2));

        horizontalLayout6->addChild(std::move(radiobutton3));
        horizontalLayout6->addChild(std::move(radiobutton_text3));   

        auto verticalLayout = gui::layout::Vertical::create();
        verticalLayout->addChild(std::move(horizontalLayout));
        verticalLayout->addChild(std::move(horizontalLayout2));
        verticalLayout->addChild(std::move(horizontalLayout3));
        verticalLayout->addChild(std::move(horizontalLayout4));   
        verticalLayout->addChild(std::move(horizontalLayout5));  
        verticalLayout->addChild(std::move(horizontalLayout6));  

        verticalLayout->setRowSize(0, gui::layout::SizeConstraint::Fixed(30.f, gui::layout::SizeConstraint::Unit::Pixels));
        verticalLayout->setRowSize(1, gui::layout::SizeConstraint::Auto());

        window->addChild(std::move(verticalLayout));

        window->setSize(sf::Vector2f(700.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2 - 150.f, 400.0f));
        window->setTitle("Oh my gosh");

        windowManager_.addWindow(std::move(window));
    });

    windowManager_.mainWindow().addChild(std::move(createLayoutWindowButton));

    auto createFocusTestWindowButton = gui::TextButton::create("Focus Test Window");
    createFocusTestWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 350.f));
    createFocusTestWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createFocusTestWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();

        auto hl1 = gui::layout::Horizontal::create();
        auto edit1 = gui::EditBox::create();
        edit1->setAlignment(gui::Alignment::Left);
        auto button1 = gui::TextButton::create("Button1");
        hl1->addChild(std::move(edit1));
        hl1->addChild(std::move(button1));

        auto hl2 = gui::layout::Horizontal::create();
        auto edit2 = gui::EditBox::create();
        edit2->setAlignment(gui::Alignment::HorizontallyCentered);
        auto button2 = gui::TextButton::create("Button2");
        hl2->addChild(std::move(edit2));
        hl2->addChild(std::move(button2));

        auto hl3 = gui::layout::Horizontal::create();
        auto edit3 = gui::EditBox::create();
        edit3->setAlignment(gui::Alignment::Right);
        auto button3 = gui::TextButton::create("Button3");
        hl3->addChild(std::move(edit3));
        hl3->addChild(std::move(button3));

        auto hl4 = gui::layout::Horizontal::create();
        auto edit4 = gui::EditBox::create();
        auto button4 = gui::TextButton::create("Button4");
        hl4->addChild(std::move(edit4));
        hl4->addChild(std::move(button4));

        auto vl = gui::layout::Vertical::create();
        vl->addChild(std::move(hl1));
        vl->addChild(std::move(hl2));
        vl->addChild(std::move(hl3));
        vl->addChild(std::move(hl4));

        window->addChild(std::move(vl));

        window->setSize(sf::Vector2f(700.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2 - 150.f, 400.0f));
        window->setTitle("I need to focus");

        windowManager_.addWindow(std::move(window));
    });

    windowManager_.mainWindow().addChild(std::move(createFocusTestWindowButton));

    auto createGridLayoutWindowButton = gui::TextButton::create("Grid Layout Window");
    createGridLayoutWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 400.f));
    createGridLayoutWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createGridLayoutWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();

        auto gridLayout = gui::layout::Grid::create(2, 3);

        auto verticalLayout = gui::layout::Vertical::create();
        auto horizontalLayout = gui::layout::Horizontal::create();

        auto positionBox = gui::layout::Vertical::create();
        auto positionLabel = gui::Label::create("Position:");
        positionLabel->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        auto positionValue = gui::EditBox::create();
        positionValue->setText("0");
        positionValue->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        auto* positionValuePtr = positionValue.get();

        auto percentageLabel = gui::Label::create("Percentage:");
        percentageLabel->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        auto ratioValue = gui::EditBox::create();
        ratioValue->setText("20");
        ratioValue->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
        auto* ratioValuePtr = ratioValue.get();

        positionBox->addChild(std::move(positionLabel));
        positionBox->addChild(std::move(positionValue));
        positionBox->addChild(std::move(percentageLabel));
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

            if (ratio.has_value()) 
            {
                if(not  gridLayoutPtr->addColumn(position, gui::layout::SizeConstraint::Fixed(ratio.value(), gui::layout::SizeConstraint::Unit::Percentage))) return;
            }
            else
            {
                if(not gridLayoutPtr->addColumn(position, gui::layout::SizeConstraint::Auto())) return;
            }

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

            if (ratio.has_value()) 
            {
                if(not  gridLayoutPtr->addRow(position, gui::layout::SizeConstraint::Fixed(ratio.value(), gui::layout::SizeConstraint::Unit::Percentage))) return;
            }
            else
            {
                if(not gridLayoutPtr->addRow(position, gui::layout::SizeConstraint::Auto())) return;
            }


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
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2 - 150.f, 400.0f));
        window->setTitle("The Grid, a digital frontier...");

        windowManager_.addWindow(std::move(window));

    });

    windowManager_.mainWindow().addChild(std::move(createGridLayoutWindowButton));

    auto createSliderWindowButton = gui::TextButton::create("Slider Window");
    createSliderWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 450.f));
    createSliderWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createSliderWindowButton->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(300.0f, 300.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2 - 150.f, 400.0f));
        window->setTitle("Slidinnn.....");

        auto gridLayout = gui::layout::Grid::create(2,4);
        auto hProgressBar = gui::ProgressBar::create();
        hProgressBar->setRange(0.f, 1.f);
        gui::ProgressBar* hProgressBarPtr = hProgressBar.get();

        gridLayout->addChild(std::move(hProgressBar));

        auto hSlider = gui::HorizontalSlider::create();
        hSlider->onValueChange([hProgressBarPtr](float value)
        {
            hProgressBarPtr->setValue(value);
        });
        gridLayout->addChild(std::move(hSlider));

        auto hProgressBar2 = gui::ProgressBar::create();
        hProgressBar2->setRange(0.f, 100.f);
        gui::ProgressBar* hProgressBarPtr2 = hProgressBar2.get();

        gridLayout->addChild(std::move(hProgressBar2));

        auto hSlider2 = gui::HorizontalSlider::create();
        hSlider2->setRange(25.f, 75.f);
        hSlider2->setStep(0.25f);
        hSlider2->onValueChange([hProgressBarPtr2](float value)
        {
            hProgressBarPtr2->setValue(value);
        });
        gridLayout->addChild(std::move(hSlider2));

        auto vProgressBar = gui::ProgressBar::create();
        vProgressBar->setRange(0.f, 1.f);
        gui::ProgressBar* vProgressBarPtr = vProgressBar.get();

        gridLayout->addChild(std::move(vProgressBar));

        auto vSlider = gui::VerticalSlider::create();
        vSlider->onValueChange([vProgressBarPtr](float value)
        {
            vProgressBarPtr->setValue(value);
        });
        gridLayout->addChild(std::move(vSlider));


        auto vProgressBar2 = gui::ProgressBar::create();
        vProgressBar2->setRange(0.f, 100.f);
        gui::ProgressBar* vProgressBarPtr2 = vProgressBar2.get();

        gridLayout->addChild(std::move(vProgressBar2));

        auto vSlider2 = gui::VerticalSlider::create();
        vSlider2->setRange(25.f, 75.f);
        vSlider2->setStep(0.25f);
        vSlider2->onValueChange([vProgressBarPtr2](float value)
        {
            vProgressBarPtr2->setValue(value);
        });
        gridLayout->addChild(std::move(vSlider2));


        window->addChild(std::move(gridLayout));
        windowManager_.addWindow(std::move(window));

    });

    windowManager_.mainWindow().addChild(std::move(createSliderWindowButton));

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
    windowManager_.render(window_);
}

}  // namespace gui_sandbox
