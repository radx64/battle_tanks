#pragma once
#include <memory>

#include "engine/Application.hpp"
#include "engine/Camera.hpp"
#include "engine/CameraController.hpp"

#include "engine/FpsCounter.hpp"

#include "engine/Renderer.hpp"
#include "engine/RigidBody.hpp"

#include "engine/math/Math.hpp"

#include "game/Context.hpp"
#include "game/entity/tank/Tank.hpp"
#include "game/entity/TracksRenderer.hpp"
#include "game/WaypointMouseController.hpp"
#include "game/Navigator.hpp"

#include "graphics/Tilemap.hpp"

#include "gui/Label.hpp"
#include "gui/Window.hpp"
#include "gui/WindowManager.hpp"
#include "gui/MouseController.hpp"

namespace game 
{

class Application : public engine::Application
{
public:
    Application();
    
protected:

    void onInit() override;
    void onClose() override;
    void onEvent(const sf::Event& event) override;
    void onUpdate(float timeStep) override;
    void onRender() override;

    void configureGUI();
    void spawnSomeTanks();
    void spawnSomeBarrelsAndCratesAndTress();
    void renderGameObjects();
    void generateProfiling();

    Context context_;
   
    engine::FpsCounter fpsCounter_;

    const sf::Vector2f cameraInitialPosition_;
    const sf::Vector2f cameraInitialSize_;
    engine::Camera camera_;
    engine::CameraController cameraController_;
    sf::View cameraView_;

    std::unique_ptr<graphics::Tilemap> tilemap_;

    bool rigidBodyDebug_{false};
    bool tankDebugMode_{false};
    
    gui::Label* measurementsTextHandle_;
    gui::Label* measurementsAverageTextHandle_;

    gui::WindowManager windowManager_;
    game::WaypointMouseController waypointMouseController_;
    gui::MouseController guiMouseController_;

    std::vector<sf::Vector2i> waypoints_;
    std::vector<std::unique_ptr<Navigator>> navigators_;

    entity::TracksRenderer tracksRenderer_;

    sf::Int32 drawTime_{0};
    sf::Time physicsTime_{};
    sf::Time navTime_{};
    sf::Time guiTime_{};

    engine::math::Average drawAverage_;
    engine::math::Average physicsAverage_;
    engine::math::Average navAverage_;
    engine::math::Average fpsAverage_;
    engine::math::Average guiAverage_;
};

}  // namespace game
