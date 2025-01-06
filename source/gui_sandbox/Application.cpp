#include "Application.hpp"

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

using namespace std::literals;

namespace gui_sandbox
{
Application::Application()
: engine::Application("GUI sandbox")
, windowManager_{sf::Vector2f{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}}
, mouseController_{&windowManager_, window_, window_.getDefaultView()}
, keyboardController_{&windowManager_}
, textEnteredController_{&windowManager_}
, timer_{5s, [](){engine::Logger::debug("2 seconds heartbeat ❤️ \n");}}
{
    auto& timerService = engine::Context::getTimerService();
    timerService.start(&timer_, engine::TimerType::Repeating);
}

void Application::onInit()
{
    gui::FontLibrary::initialize();

    window_.setFramerateLimit(60);
    window_.setVerticalSyncEnabled(true);

    generateBackground();

    mouseHandler_.subscribe(&mouseController_);
    keyboardHandler_.subscribe(&keyboardController_);
    keyboardHandler_.subscribe(&textEnteredController_);

    auto quitButton = std::make_unique<gui::Button>("Quit");
    quitButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 50.f));
    quitButton->setSize(sf::Vector2f(250.f, 30.f));
    quitButton->onClick([this](){engine::Logger::info("Quitting..."); Application::close();});
    windowManager_.mainWindow().addChild(std::move(quitButton));

    auto guiDebug = std::make_unique<gui::Button>("GUI DEBUG");
    guiDebug->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, Config::WINDOW_HEIGHT - 50.f));
    guiDebug->setSize(sf::Vector2f(250.f, 30.f));
    guiDebug->onClick([](){gui::debug::toggle();});
    windowManager_.mainWindow().addChild(std::move(guiDebug));

    auto createEmptyWindowButton = std::make_unique<gui::Button>("Create Empty Window");
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

    auto createSimpleWindowButton = std::make_unique<gui::Button>("Create Simple Window");
    createSimpleWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 150.f));
    createSimpleWindowButton->setSize(sf::Vector2f(250.f, 30.f));

    createSimpleWindowButton->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        auto simpleButton = std::make_unique<gui::Button>("THIS IS BUTTON!");
        simpleButton->setSize({100.f, 100.f});
        simpleButton->setPosition({10.f, 10.f});

        auto fillLayout = std::make_unique<gui::FillLayout>();

        fillLayout->addChild(std::move(simpleButton));

        window->addChild(std::move(fillLayout));

        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("So simple!");

        windowManager_.addWindow(std::move(window));

    });

    windowManager_.mainWindow().addChild(std::move(createSimpleWindowButton));

    auto createProgressWindowButton = std::make_unique<gui::Button>("Create ProgressBar Window");
    createProgressWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 200.f));
    createProgressWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createProgressWindowButton->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("I see some progress :D");

        auto verticalLayout = std::make_unique<gui::VerticalLayout>();
        auto minusButton = std::make_unique<gui::Button>("-");
        auto progressBar = std::make_unique<gui::ProgressBar>();
        auto plusButton = std::make_unique<gui::Button>("+");


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

    auto createEditboxWindowButton = std::make_unique<gui::Button>("Create Edit Box Window");
    createEditboxWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 250.f));
    createEditboxWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createEditboxWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("I can now read text from keyboard :D");

        auto verticalLayout = std::make_unique<gui::VerticalLayout>();
        auto button = std::make_unique<gui::Button>("CLICK HERE");
        auto editBox = std::make_unique<gui::EditBox>();
        auto editBox2 = std::make_unique<gui::EditBox>();

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

    auto createLayoutWindowButton = std::make_unique<gui::Button>("Create Layout Window");
    createLayoutWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 300.f));
    createLayoutWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createLayoutWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();
        auto horizontalLayout = std::make_unique<gui::HorizontalLayout>();
        auto helloButton = std::make_unique<gui::Button>("HELLO");
        auto worldButton = std::make_unique<gui::Button>("WORLD");

        auto horizontalLayout2 = std::make_unique<gui::HorizontalLayout>();
        auto somethingButton = std::make_unique<gui::Button>("SOMETHING");
        auto cookingButton = std::make_unique<gui::Button>("IS COOKING");
        auto hardButton = std::make_unique<gui::Button>("HARD");

        helloButton->onClick([](){engine::Logger::info("Hello?");});
        worldButton->onClick([](){engine::Logger::info("Is it me you looking for?");});

        horizontalLayout->addChild(std::move(helloButton));
        horizontalLayout->addChild(std::move(worldButton));

        horizontalLayout2->addChild(std::move(somethingButton));

        auto verticalLayout2 = std::make_unique<gui::VerticalLayout>();
        verticalLayout2->addChild(std::move(cookingButton));
        verticalLayout2->addChild(std::move(hardButton));

        horizontalLayout2->addChild(std::move(verticalLayout2));

        auto verticalLayout = std::make_unique<gui::VerticalLayout>();

        verticalLayout->addChild(std::move(horizontalLayout));
        verticalLayout->addChild(std::move(horizontalLayout2));

        window->addChild(std::move(verticalLayout));

        window->setSize(sf::Vector2f(700.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("Oh my gosh");

        windowManager_.addWindow(std::move(window));

    });

    windowManager_.mainWindow().addChild(std::move(createLayoutWindowButton));

    auto createFocusTestWindowButton = std::make_unique<gui::Button>("Create Focus Test Window");
    createFocusTestWindowButton->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 350.f));
    createFocusTestWindowButton->setSize(sf::Vector2f(250.f, 30.f));
    createFocusTestWindowButton->onClick([this](){

        auto window = std::make_unique<gui::Window>();

        auto hl1 = std::make_unique<gui::HorizontalLayout>();
        auto edit1 = std::make_unique<gui::EditBox>();
        auto button1 = std::make_unique<gui::Button>("Button1");
        hl1->addChild(std::move(edit1));
        hl1->addChild(std::move(button1));

        auto hl2 = std::make_unique<gui::HorizontalLayout>();
        auto edit2 = std::make_unique<gui::EditBox>();
        auto button2 = std::make_unique<gui::Button>("Button2");
        hl2->addChild(std::move(edit2));
        hl2->addChild(std::move(button2));

        auto hl3 = std::make_unique<gui::HorizontalLayout>();
        auto edit3 = std::make_unique<gui::EditBox>();
        auto button3 = std::make_unique<gui::Button>("Button3");
        hl3->addChild(std::move(edit3));
        hl3->addChild(std::move(button3));

        auto hl4 = std::make_unique<gui::HorizontalLayout>();
        auto edit4 = std::make_unique<gui::EditBox>();
        auto button4 = std::make_unique<gui::Button>("Button4");
        hl4->addChild(std::move(edit4));
        hl4->addChild(std::move(button4));

        auto vl = std::make_unique<gui::VerticalLayout>();
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

    auto leftLabel = std::make_unique<gui::Label>("Left aligned label");
    leftLabel->setPosition({100.f, 20.f});
    leftLabel->setSize({400.f, 40.f});
    leftLabel->setAlignment(gui::Alignment::Left);

    auto hcenterLabel = std::make_unique<gui::Label>("Horizontally centered label");
    hcenterLabel->setPosition({100.f, 80.f});
    hcenterLabel->setSize({400.f, 40.f});
    hcenterLabel->setAlignment(gui::Alignment::HorizontallyCentered);

    auto rightLabel = std::make_unique<gui::Label>("Right aligned label");
    rightLabel->setPosition({100.f, 140.f});
    rightLabel->setSize({400.f, 40.f});
    rightLabel->setAlignment(gui::Alignment::Right);

    windowManager_.mainWindow().addChild(std::move(leftLabel));
    windowManager_.mainWindow().addChild(std::move(hcenterLabel));
    windowManager_.mainWindow().addChild(std::move(rightLabel));

    auto topLabel = std::make_unique<gui::Label>("Top aligned label");
    topLabel->setPosition({700.f, 20.f});
    topLabel->setSize({400.f, 40.f});
    topLabel->setAlignment(gui::Alignment::Top);

    auto vcenterLabel = std::make_unique<gui::Label>("Vertically centered label");
    vcenterLabel->setPosition({700.f, 80.f});
    vcenterLabel->setSize({400.f, 40.f});
    vcenterLabel->setAlignment(gui::Alignment::VerticallyCentered);

    auto bottomLabel = std::make_unique<gui::Label>("Bottom aligned label");
    bottomLabel->setPosition({700.f, 140.f});
    bottomLabel->setSize({400.f, 40.f});
    bottomLabel->setAlignment(gui::Alignment::Bottom);

    windowManager_.mainWindow().addChild(std::move(topLabel));
    windowManager_.mainWindow().addChild(std::move(vcenterLabel));
    windowManager_.mainWindow().addChild(std::move(bottomLabel));

    auto multilineTopAlignedLabel = std::make_unique<gui::Label>("Top multiline\nlabel is here\nand a bit there");
    multilineTopAlignedLabel->setPosition({100.f, 300.f});
    multilineTopAlignedLabel->setSize({400.f, 40.f});
    multilineTopAlignedLabel->setAlignment(gui::Alignment::Top);

    auto multilineCenterAlignedLabel = std::make_unique<gui::Label>("Centered multiline\nlabel is here\nand a bit there");
    multilineCenterAlignedLabel->setPosition({100.f, 400.f});
    multilineCenterAlignedLabel->setSize({400.f, 40.f});
    multilineCenterAlignedLabel->setAlignment(gui::Alignment::VerticallyCentered);

    auto multilineBottomAlignedLabel = std::make_unique<gui::Label>("Bottom multiline\nlabel is here\nand a bit there");
    multilineBottomAlignedLabel->setPosition({100.f, 500.f});
    multilineBottomAlignedLabel->setSize({400.f, 40.f});
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
