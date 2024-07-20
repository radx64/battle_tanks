#include "Application.hpp"

#include "Config.hpp"
#include "gui/Button.hpp"
#include "gui/Window.hpp"
#include "gui/Layout.hpp"
#include "gui/FontLibrary.hpp"

#include "gui_sandbox/MouseController.hpp"

#include <iostream>

namespace gui_sandbox
{
Application::Application()
: engine::Application("GUI sandbox")
{}

void Application::onInit()
{
    gui::FontLibrary::initialize();

    window_manager_ = std::make_unique<gui::WindowManager>(sf::Vector2f{Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT});
    mouse_controller_ = std::make_unique<gui_sandbox::MouseController>(window_manager_.get());
    mouse_handler_.subscribe(mouse_controller_.get());

    auto quit_button = std::make_unique<gui::Button>("Quit");
    quit_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 300.f, 100.f));
    quit_button->setSize(sf::Vector2f(250.f, 30.f));
    quit_button->onClick([this](){std::cout << "Quitting...\n"; Application::close();});   
    window_manager_->mainWindow()->addChild(std::move(quit_button));

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


        //TODO: is is easy to mix-up addComponent and addChild
        // maybe override of addChild method in Window class?
        window->addComponent(std::move(fillLayout));

        window->setSize(sf::Vector2f(400.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("So simple!");

        window_manager_->addWindow(std::move(window));

    });

    window_manager_->mainWindow()->addChild(std::move(create_window_button));

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

        window->addComponent(std::move(vertical_layout));

        window->setSize(sf::Vector2f(700.0f, 400.0f));
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f));
        window->setTitle("Oh my gosh");

        window_manager_->addWindow(std::move(window));

    });
    
    window_manager_->mainWindow()->addChild(std::move(create_layout_window_button));

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

    window_manager_->mainWindow()->addChild(std::move(left_label));
    window_manager_->mainWindow()->addChild(std::move(hcenter_label));
    window_manager_->mainWindow()->addChild(std::move(right_label));

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

    window_manager_->mainWindow()->addChild(std::move(top_label));
    window_manager_->mainWindow()->addChild(std::move(vcenter_label));
    window_manager_->mainWindow()->addChild(std::move(bottom_label));

}

void Application::onClose()
{
}

void Application::onEvent(const sf::Event& event)
{
    (void) event;
}

void Application::onUpdate(float timeStep)
{
    (void) timeStep;
}

void Application::onRender()
{
    window_manager_->render(window_);
}

}  // namespace gui_sandbox
