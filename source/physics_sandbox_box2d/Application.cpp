#include "Application.hpp"

#include "Config.hpp"

#include "engine/RigidBodyDebugRenderer.hpp"
#include "graphics/TextureLibrary.hpp"

namespace sandbox
{

Application::Application()
: engine::Application{"Physics sandbox box2d", "Physics sandbox box2d", {Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT}}
, mouseController_{
    [this](float x, float y) { spawnCircle(x, y, 25); }, 
    [this](float x, float y) { spawnSquare(x, y, 50); }}
, keyboardController_{
    [this]() { debugDraw_ = !debugDraw_; }}
, worldId_{}
{}

constexpr uint32_t NUM_OF_CIRCLES = 64;
const float SCALE = 30.0f; // 1 meter = 30 pixels


void Application::spawnCircle(float x, float y, float radius)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2{x / SCALE, y / SCALE};
    b2BodyId bodyId = b2CreateBody(worldId_, &bodyDef);

    b2Circle circle;
    circle.center = b2Vec2{0.0f, 0.0f}; // Center relative to the body position
    circle.radius = radius / SCALE;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    b2CreateCircleShape(bodyId, &shapeDef, &circle);

    bodies_.push_back(bodyId);
}

void Application::spawnSquare(float x, float y, float side)
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2{x / SCALE, y / SCALE};
    b2BodyId bodyId = b2CreateBody(worldId_, &bodyDef);

    b2Polygon box = b2MakeBox(side / (2 * SCALE), side / (2 * SCALE)); // half-width and half-height

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;
    b2CreatePolygonShape(bodyId, &shapeDef, &box);

    bodies_.push_back(bodyId);
}


void Application::onInit()
{
    mouseHandler_.subscribe(&mouseController_);
    keyboardHandler_.subscribe(&keyboardController_);

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, 10.0f};
    worldId_ = b2CreateWorld(&worldDef);

    // ground
    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = b2Vec2{1000.f / SCALE, 1000.0f / SCALE};

    b2BodyId groundId = b2CreateBody(worldId_, &groundBodyDef);
    bodies_.push_back(groundId);

    b2Polygon groundBox = b2MakeBox(1000.0f / SCALE, 20.0f / SCALE);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    // left wall
    b2BodyDef leftWallBodyDef = b2DefaultBodyDef();
    leftWallBodyDef.position = b2Vec2{0.0f / SCALE, 500.0f / SCALE};
    b2BodyId leftWallId = b2CreateBody(worldId_, &leftWallBodyDef);
    bodies_.push_back(leftWallId);  

    b2Polygon leftWallBox = b2MakeBox(20.0f / SCALE, 500.0f / SCALE);
    b2ShapeDef leftWallShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(leftWallId, &leftWallShapeDef, &leftWallBox);

    // right wall
    b2BodyDef rightWallBodyDef = b2DefaultBodyDef();
    rightWallBodyDef.position = b2Vec2{1900.0f / SCALE, 500.0f / SCALE};
    b2BodyId rightWallId = b2CreateBody(worldId_, &rightWallBodyDef);
    bodies_.push_back(rightWallId);

    b2Polygon rightWallBox = b2MakeBox(20.0f / SCALE, 500.0f / SCALE);
    b2ShapeDef rightWallShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(rightWallId, &rightWallShapeDef, &rightWallBox);


    for (uint32_t idx = 0; idx < NUM_OF_CIRCLES; ++idx)
    {
        spawnCircle(400 + 15 * idx, 40 * idx, 25);
    }

    boxTexture_ = graphics::TextureLibrary::instance().get("crate_wood");
    circleTexture_ = graphics::TextureLibrary::instance().get("barrel_red");

    boxSprite_.setTexture(boxTexture_);
    circleSprite_.setTexture(circleTexture_);
}

void Application::onClose()
{
    b2DestroyWorld(worldId_);
}

void Application::onEvent(const sf::Event& event)
{
    (void) event;
}

void Application::onUpdate(float time_step)
{
    (void) time_step;
    float time_stepConstant = 1.0f / 60.0f;
    float subStepCount = 4;
    b2World_Step(worldId_, time_stepConstant, subStepCount);
}

void Application::drawBody(b2BodyId bodyId) 
{
    int shapeCount = b2Body_GetShapeCount(bodyId);
    if (shapeCount == 0) return;

    std::vector<b2ShapeId> shapes(shapeCount);
    b2Body_GetShapes(bodyId, shapes.data(), shapeCount);

    b2Transform transform = b2Body_GetTransform(bodyId);

    for (b2ShapeId shapeId : shapes) {
        b2ShapeType type = b2Shape_GetType(shapeId);

        if (type == b2ShapeType::b2_polygonShape) 
        {
            b2Polygon polygon = b2Shape_GetPolygon(shapeId);

            if (polygon.count == 4) // render only boxes with textures
            {
                float targetWidthInPixels = ((polygon.vertices[1].x  - polygon.vertices[0].x)) * SCALE;
                float targetHeightInPixels = ((polygon.vertices[2].y - polygon.vertices[1].y)) * SCALE;
                
                sf::FloatRect bounds = boxSprite_.getLocalBounds();
                boxSprite_.setScale(
                    targetWidthInPixels / bounds.width,
                    targetHeightInPixels / bounds.height
                );

                b2Vec2 worldPos = b2TransformPoint(transform, polygon.vertices[0]);
                boxSprite_.setPosition(worldPos.x * SCALE, worldPos.y * SCALE);
                
                b2Rot rotation = transform.q;
                float angleInRadians = b2Rot_GetAngle(rotation);
                
                boxSprite_.setRotation(angleInRadians * 180.0f / 3.14159265f);
                window_.draw(boxSprite_);
            }

            if (!debugDraw_) continue;

            sf::VertexArray lines(sf::LineStrip, polygon.count + 1);

            for (int i = 0; i < polygon.count; ++i) {
                b2Vec2 worldPos = b2TransformPoint(transform, polygon.vertices[i]);
                lines[i].position = sf::Vector2f(worldPos.x * SCALE, worldPos.y * SCALE);
                if(b2Body_IsAwake(bodyId))
                {
                    lines[i].color = sf::Color::Blue;
                }
                else
                {
                    lines[i].color = sf::Color(128, 128, 128, 128);
                }
            }
            lines[polygon.count] = lines[0]; // Close outline loop
            window_.draw(lines);
        }
        else if (type == b2ShapeType::b2_circleShape) 
        {
            b2Circle circle = b2Shape_GetCircle(shapeId);

            b2Vec2 worldPos = b2TransformPoint(transform, circle.center);
            circleSprite_.setPosition(worldPos.x * SCALE, worldPos.y * SCALE);            

            float targetWidthInPixels = ((circle.radius * 2) * SCALE);
            float targetHeightInPixels = ((circle.radius * 2) * SCALE);

            circleSprite_.setOrigin(circleTexture_.getSize().x / 2.0f, circleTexture_.getSize().y / 2.0f);

            sf::FloatRect bounds = circleSprite_.getLocalBounds();
            circleSprite_.setScale(
                targetWidthInPixels / bounds.width,
                targetHeightInPixels / bounds.height
            );

            b2Rot rotation = transform.q;
            float angleInRadians = b2Rot_GetAngle(rotation);
            circleSprite_.setRotation(angleInRadians * 180.0f / 3.14159265f);         
            window_.draw(circleSprite_);

            if (!debugDraw_) continue;

            float pixelRadius = circle.radius * SCALE;

            sf::CircleShape sfCircle(pixelRadius);
            if (b2Body_IsAwake(bodyId))
            {
                sfCircle.setOutlineColor(sf::Color::Red);
            } else {
                sfCircle.setOutlineColor(sf::Color(128, 128, 128, 128));
            }

            sfCircle.setOutlineThickness(1.0f);
            sfCircle.setFillColor(sf::Color::Transparent);
            
            sfCircle.setOrigin(pixelRadius, pixelRadius);
            sfCircle.setPosition(worldPos.x * SCALE, worldPos.y * SCALE);

            sf::VertexArray radiusLine(sf::Lines, 2);
            radiusLine[0].position = sfCircle.getPosition();
            radiusLine[0].color = sf::Color::Green;

            b2Vec2 edgePoint = b2TransformPoint(transform, b2Vec2{circle.center.x + circle.radius, circle.center.y});
            radiusLine[1].position = sf::Vector2f(edgePoint.x * SCALE, edgePoint.y * SCALE);
            radiusLine[1].color = sf::Color::Green;

            window_.draw(sfCircle);
            window_.draw(radiusLine);
        }
    }
}

void Application::onRender()
{
    for (b2BodyId bodyId : bodies_) 
    {
        drawBody(bodyId);
    }
}

}  // namespace sandbox
