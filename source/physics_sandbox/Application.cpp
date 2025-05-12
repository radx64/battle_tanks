#include "Application.hpp"

#include "Circle.hpp"

#include "Config.hpp"

#include "engine/RigidBodyDebugRenderer.hpp"

namespace sandbox
{

Application::Application()
: engine::Application{"Physics sandbox", "Physics sandbox", {Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}}
, mouseController_{scene_}
{}

constexpr uint32_t NUM_OF_CIRCLES = 64;

void Application::onInit()
{
    mouseHandler_.subscribe(&mouseController_);

    for (uint32_t idx = 0; idx < NUM_OF_CIRCLES; ++idx)
    {
        scene_.spawnObject(std::make_unique<Circle>(
            idx, 
            400 + 10 * idx,
            40 * idx,
            25,
            100,
            engine::RigidBody::Type::DYNAMIC));
    }

    // BOTTOM
    for (uint32_t idx = 0; idx < 40; ++idx)
    {
        scene_.spawnObject(std::make_unique<Circle>(
            100 + idx, 
            50 * idx,
            900,
            25,
            100,
            engine::RigidBody::Type::STATIC));
    }
    // LEFT and RIGHT
    for (uint32_t idx = 0; idx < 20; ++idx)
    {
        scene_.spawnObject(std::make_unique<Circle>(
            200 + idx, 
            0,
            50 * idx,
            25,
            100,
            engine::RigidBody::Type::STATIC));

        scene_.spawnObject(std::make_unique<Circle>(
            300 + idx, 
            1900,
            50 * idx,
            25,
            100,
            engine::RigidBody::Type::STATIC));
    }

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

    for (auto& object : scene_.objects())
    {
        auto& rigidBody = object->getRigidBody();
        if (rigidBody.type_ == engine::RigidBody::Type::DYNAMIC)
        {
            object->getRigidBody().applyForce(sf::Vector2f{.0f, 10.5f});
        }
        object->update(scene_, timeStep);
    }

    collisionSolver_.evaluateCollisions();
}

void Application::onRender()
{
    engine::RigidBodyDebugRenderer::debug(scene_, window_);
}

}  // namespace sandbox
