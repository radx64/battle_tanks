#pragma once

#include <memory>

#include "engine/Camera.hpp"
#include "engine/CameraController.hpp"
#include "engine/FpsCounter.hpp"
#include "engine/math/Math.hpp"
#include "engine/Profiler.hpp"
#include "engine/Renderer.hpp"
#include "engine/RigidBody.hpp"

#include "game/entity/tank/Tank.hpp"
#include "game/entity/TracksRenderer.hpp"
#include "game/Navigator.hpp"
#include "game/WaypointMouseController.hpp"

#include "graphics/Tilemap.hpp"

#include "gui/Application.hpp"
#include "gui/Label.hpp"
#include "gui/Window.hpp"
#include "gui/WindowManager.hpp"
#include "gui/MouseController.hpp"

namespace game 
{

class Application : public gui::Application
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

    game::WaypointMouseController waypointMouseController_;

    std::vector<sf::Vector2i> waypoints_;
    std::vector<std::unique_ptr<Navigator>> navigators_;

    entity::TracksRenderer tracksRenderer_;
    engine::math::Average fpsAverage_;

    engine::Profiler drawProfiler_;
    engine::Profiler physicsProfiler_;
    engine::Profiler navProfiler_;
    engine::Profiler guiProfiler_;
};

}  // namespace game
