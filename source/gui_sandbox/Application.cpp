#include "Application.hpp"

#include "Config.hpp"
#include "gui/Button.hpp"
#include "gui/Window.hpp"
#include "gui/Layout.hpp"
#include "gui/FontLibrary.hpp"

#include "gui_sandbox/MouseController.hpp"

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
    quit_button->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 100.f), gui::Alignment::LEFT);
    quit_button->setSize(sf::Vector2f(150.f, 30.f));
    quit_button->onClick([this](){std::cout << "Quitting...\n"; Application::close();});   
    window_manager_->mainWindow()->addChild(std::move(quit_button));

    auto create_window = std::make_unique<gui::Button>("Create Window");
    create_window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH - 200.f, 300.f), gui::Alignment::LEFT);
    create_window->setSize(sf::Vector2f(150.f, 30.f));
    create_window->onClick([this](){

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
        window->setPosition(sf::Vector2f(Config::WINDOW_WIDTH/2, 400.0f), gui::Alignment::CENTERED);
        window->setTitle("Oh my gosh");

        window_manager_->addWindow(std::move(window));

    });
    
    window_manager_->mainWindow()->addChild(std::move(create_window));   
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
