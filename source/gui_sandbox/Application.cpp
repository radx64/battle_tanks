#include "Application.hpp"

#include "Config.hpp"

#include "gui/Debug.hpp"
#include "gui/Button.hpp"
#include "gui/EditBox.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/Layout.hpp"
#include "gui/Window.hpp"
#include "gui/ProgressBar.hpp"

#include <iostream>

namespace gui_sandbox
{
Application::Application()
: engine::Application("GUI sandbox")
, window_manager_{sf::Vector2f{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}}
, mouse_controller_{&window_manager_, window_, window_.getDefaultView()}
, keyboard_controller_{&window_manager_}
, text_entered_controller_{&window_manager_}
{
}

void Application::onInit()
{
    gui::FontLibrary::initialize();

    window_.setFramerateLimit(60);
    window_.setVerticalSyncEnabled(true);

    generateBackground();

    mouse_handler_.subscribe(&mouse_controller_);
    keyboard_handler_.subscribe(&keyboard_controller_);
    keyboard_handler_.subscribe(&text_entered_controller_);

    auto quit_button = std::make_unique<gui::Button>("Quit");
    quit_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 50.f));
    quit_button->setSize(sf::Vector2f(250.f, 30.f));
    quit_button->onClick([this](){std::cout << "Quitting...\n"; Application::close();});
    window_manager_.mainWindow().addChild(std::move(quit_button));

    auto gui_debug = std::make_unique<gui::Button>("GUI DEBUG");
    gui_debug->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, Config::WINDOW_HEIGHT - 50.f));
    gui_debug->setSize(sf::Vector2f(250.f, 30.f));
    gui_debug->onClick([](){gui::debug::toggle();});
    window_manager_.mainWindow().addChild(std::move(gui_debug));

    auto create_empty_window_button = std::make_unique<gui::Button>("Create Empty Window");
    create_empty_window_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 100.f));
    create_empty_window_button->setSize(sf::Vector2f(250.f, 30.f));

    create_empty_window_button->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("Nothing here!");

        window_manager_.addWindow(std::move(window));
    });

    window_manager_.mainWindow().addChild(std::move(create_empty_window_button));

    auto create_window_button = std::make_unique<gui::Button>("Create Simple Window");
    create_window_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 150.f));
    create_window_button->setSize(sf::Vector2f(250.f, 30.f));

    create_window_button->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        auto simple_button = std::make_unique<gui::Button>("THIS IS BUTTON!");
        simple_button->setSize({100.f, 100.f});
        simple_button->setPosition({10.f, 10.f});

        auto fillLayout = std::make_unique<gui::FillLayout>();

        fillLayout->addChild(std::move(simple_button));

        window->addChild(std::move(fillLayout));

        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("So simple!");

        window_manager_.addWindow(std::move(window));

    });

    window_manager_.mainWindow().addChild(std::move(create_window_button));

    auto create_progress_window_button = std::make_unique<gui::Button>("Create ProgressBar Window");
    create_progress_window_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 200.f));
    create_progress_window_button->setSize(sf::Vector2f(250.f, 30.f));
    create_progress_window_button->onClick([this](){
        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("I see some progress :D");

        auto vertical_layout = std::make_unique<gui::VerticalLayout>();
        auto minus_button = std::make_unique<gui::Button>("-");
        auto progress_bar = std::make_unique<gui::ProgressBar>();
        auto plus_button = std::make_unique<gui::Button>("+");


        progress_bar->setRange(0.f, 25.f);

        minus_button->onClick([progress_bar_ptr = progress_bar.get()]{
            progress_bar_ptr->setValue(progress_bar_ptr->getValue()-1.f);
        });

        plus_button->onClick([progress_bar_ptr = progress_bar.get()]{
            progress_bar_ptr->setValue(progress_bar_ptr->getValue()+1.f);
        });

        vertical_layout->addChild(std::move(minus_button));
        vertical_layout->addChild(std::move(progress_bar));
        vertical_layout->addChild(std::move(plus_button));

        window->addChild(std::move(vertical_layout));

        window_manager_.addWindow(std::move(window));

    });

    window_manager_.mainWindow().addChild(std::move(create_progress_window_button));

    auto create_editbox_window_button = std::make_unique<gui::Button>("Create Edit Box Window");
    create_editbox_window_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 250.f));
    create_editbox_window_button->setSize(sf::Vector2f(250.f, 30.f));
    create_editbox_window_button->onClick([this](){

        auto window = std::make_unique<gui::Window>();
        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("I can now read text from keyboard :D");

        auto vertical_layout = std::make_unique<gui::VerticalLayout>();
        auto button = std::make_unique<gui::Button>("CLICK HERE");
        auto edit_box = std::make_unique<gui::EditBox>();
        auto edit_box_2 = std::make_unique<gui::EditBox>();

        button->onClick([edit_box_ptr = edit_box.get(), edit_box_2_ptr = edit_box_2.get(), button_ptr = button.get()]{
            button_ptr->setText(edit_box_ptr->getText() + " | " + edit_box_2_ptr->getText());
        });

        vertical_layout->addChild(std::move(button));
        vertical_layout->addChild(std::move(edit_box));
        vertical_layout->addChild(std::move(edit_box_2));

        window->addChild(std::move(vertical_layout));

        window_manager_.addWindow(std::move(window));

    });

    window_manager_.mainWindow().addChild(std::move(create_editbox_window_button));

    auto create_layout_window_button = std::make_unique<gui::Button>("Create Layout Window");
    create_layout_window_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 300.f));
    create_layout_window_button->setSize(sf::Vector2f(250.f, 30.f));
    create_layout_window_button->onClick([this](){

        auto window = std::make_unique<gui::Window>();
        auto horizontal_layout = std::make_unique<gui::HorizontalLayout>();
        auto hello_button = std::make_unique<gui::Button>("HELLO");
        auto world_button = std::make_unique<gui::Button>("WORLD");

        auto horizontal_layout2 = std::make_unique<gui::HorizontalLayout>();
        auto something_button = std::make_unique<gui::Button>("SOMETHING");
        auto cooking_button = std::make_unique<gui::Button>("IS COOKING");
        auto hard_button = std::make_unique<gui::Button>("HARD");

        hello_button->onClick([](){std::cout << "Hello?" << std::endl;});
        world_button->onClick([](){std::cout << "Is it me you looking for?" << std::endl;});

        horizontal_layout->addChild(std::move(hello_button));
        horizontal_layout->addChild(std::move(world_button));

        horizontal_layout2->addChild(std::move(something_button));

        auto vertical_layout2 = std::make_unique<gui::VerticalLayout>();
        vertical_layout2->addChild(std::move(cooking_button));
        vertical_layout2->addChild(std::move(hard_button));

        horizontal_layout2->addChild(std::move(vertical_layout2));

        auto vertical_layout = std::make_unique<gui::VerticalLayout>();

        vertical_layout->addChild(std::move(horizontal_layout));
        vertical_layout->addChild(std::move(horizontal_layout2));

        window->addChild(std::move(vertical_layout));

        window->setSize(sf::Vector2f(700.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("Oh my gosh");

        window_manager_.addWindow(std::move(window));

    });

    window_manager_.mainWindow().addChild(std::move(create_layout_window_button));

    auto left_label = std::make_unique<gui::Label>("Left aligned label");
    left_label->setPosition({100.f, 20.f});
    left_label->setSize({400.f, 40.f});
    left_label->setAlignment(gui::Alignment::Left);

    auto hcenter_label = std::make_unique<gui::Label>("Horizontally centered label");
    hcenter_label->setPosition({100.f, 80.f});
    hcenter_label->setSize({400.f, 40.f});
    hcenter_label->setAlignment(gui::Alignment::HorizontallyCentered);

    auto right_label = std::make_unique<gui::Label>("Right aligned label");
    right_label->setPosition({100.f, 140.f});
    right_label->setSize({400.f, 40.f});
    right_label->setAlignment(gui::Alignment::Right);

    window_manager_.mainWindow().addChild(std::move(left_label));
    window_manager_.mainWindow().addChild(std::move(hcenter_label));
    window_manager_.mainWindow().addChild(std::move(right_label));

    auto top_label = std::make_unique<gui::Label>("Top aligned label");
    top_label->setPosition({700.f, 20.f});
    top_label->setSize({400.f, 40.f});
    top_label->setAlignment(gui::Alignment::Top);

    auto vcenter_label = std::make_unique<gui::Label>("Vertically centered label");
    vcenter_label->setPosition({700.f, 80.f});
    vcenter_label->setSize({400.f, 40.f});
    vcenter_label->setAlignment(gui::Alignment::VerticallyCentered);

    auto bottom_label = std::make_unique<gui::Label>("Bottom aligned label");
    bottom_label->setPosition({700.f, 140.f});
    bottom_label->setSize({400.f, 40.f});
    bottom_label->setAlignment(gui::Alignment::Bottom);

    window_manager_.mainWindow().addChild(std::move(top_label));
    window_manager_.mainWindow().addChild(std::move(vcenter_label));
    window_manager_.mainWindow().addChild(std::move(bottom_label));


    auto multiline_top_aligned_label = std::make_unique<gui::Label>("Top multiline\nlabel is here\nand a bit there");
    multiline_top_aligned_label->setPosition({100.f, 300.f});
    multiline_top_aligned_label->setSize({400.f, 40.f});
    multiline_top_aligned_label->setAlignment(gui::Alignment::Top);

    auto multiline_center_aligned_label = std::make_unique<gui::Label>("Centered multiline\nlabel is here\nand a bit there");
    multiline_center_aligned_label->setPosition({100.f, 400.f});
    multiline_center_aligned_label->setSize({400.f, 40.f});
    multiline_center_aligned_label->setAlignment(gui::Alignment::VerticallyCentered);

    auto multiline_bottom_aligned_label = std::make_unique<gui::Label>("Bottom multiline\nlabel is here\nand a bit there");
    multiline_bottom_aligned_label->setPosition({100.f, 500.f});
    multiline_bottom_aligned_label->setSize({400.f, 40.f});
    multiline_bottom_aligned_label->setAlignment(gui::Alignment::Bottom);

    window_manager_.mainWindow().addChild(std::move(multiline_top_aligned_label));
    window_manager_.mainWindow().addChild(std::move(multiline_center_aligned_label));
    window_manager_.mainWindow().addChild(std::move(multiline_bottom_aligned_label));
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

    sf::RectangleShape outer_box{};
    outer_box.setPosition(EDGE_OFFSET, EDGE_OFFSET);
    outer_box.setSize(sf::Vector2f{Config::WINDOW_WIDTH - 2 * EDGE_OFFSET, Config::WINDOW_HEIGHT -  2*EDGE_OFFSET});
    outer_box.setOutlineThickness(LINE_THICKNESS);
    outer_box.setFillColor(sf::Color::Transparent);
    outer_box.setOutlineColor(sf::Color(255,255,255,240));
    backgroundTexture_.draw(outer_box);

    for (auto x = 1; x < X_STEPS; ++x)
    {
        auto x_coord = x * ((Config::WINDOW_WIDTH - 2*EDGE_OFFSET) / X_STEPS) + EDGE_OFFSET;
        sf::RectangleShape line{};
        line.setPosition(x_coord, EDGE_OFFSET);
        line.setSize(sf::Vector2f{LINE_THICKNESS/4.f, Config::WINDOW_HEIGHT -  2*EDGE_OFFSET});
        line.setFillColor(sf::Color(255,255,255,200));
        backgroundTexture_.draw(line);
    }

    for (auto y = 1; y < Y_STEPS; ++y)
    {
        auto y_coord = y * ((Config::WINDOW_HEIGHT- 2*EDGE_OFFSET) / Y_STEPS) + EDGE_OFFSET;
        sf::RectangleShape line{};
        line.setPosition(EDGE_OFFSET, y_coord);
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
    window_manager_.render(window_);
}

}  // namespace gui_sandbox
