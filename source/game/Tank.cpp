#include "game/Tank.hpp"

#include <algorithm>
#include <cmath>

#include "game/Context.hpp"
#include "gui/FontLibrary.hpp"
#include "graphics/Particles.hpp"
#include "graphics/TextureLibrary.hpp"
#include "math/Math.hpp"

namespace game 
{

constexpr float TANK_BRAKE_FORCE = 0.1;
constexpr float TANK_ACCELERATION = 6.0;
constexpr float TANK_ROTATION_SPEED = 200.0;

constexpr float TANK_RADIUS = 25;
constexpr float TANK_MASS = 50;
constexpr float GROUND_DRAG_COEEF = 0.97;

bool Tank::DEBUG_{false};

void Tank::setDebug(bool is_enabled)
{
    DEBUG_ = is_enabled;
}

void Tank::drawDebugInfo(sf::RenderWindow& renderWindow)
{
    // Collision circle
    sf::CircleShape boundary(TANK_RADIUS, 12);
    boundary.setFillColor(sf::Color(0, 0, 0, 0));
    boundary.setOutlineThickness(1);
    boundary.setOutlineColor(sf::Color(0, 0, 255));
    boundary.setOrigin(TANK_RADIUS, TANK_RADIUS);
    boundary.setPosition(x_, y_);
    renderWindow.draw(boundary);

    sf::Text debug_text;
    debug_text.setFont(gui::FontLibrary::get("armata"));
    debug_text.setPosition(x_ + 40, y_ - 20);
    debug_text.setCharacterSize(15);
    debug_text.setFillColor(sf::Color::Black);

    debug_text.setString("SPD: " + std::to_string(fabs(velocity_.x + velocity_.y)) + "\n" + 
        "ROT: " + std::to_string(current_direction_) + "\n" + 
        "THR: " + std::to_string(current_throttle_));
    renderWindow.draw(debug_text);

    // Velocity vectors
    sf::Vertex velocity_vector[] =
    {
        sf::Vertex(sf::Vector2f(x_, y_)),
        sf::Vertex(sf::Vector2f(x_, y_)+ (velocity_))
    };
    renderWindow.draw(velocity_vector, 2, sf::Lines);

    sf::Vertex velocity_x_vector[] =
    {
        sf::Vertex(sf::Vector2f(x_, y_), sf::Color::Red),
        sf::Vertex(sf::Vector2f(x_+velocity_.x, y_), sf::Color::Red)
    };

    renderWindow.draw(velocity_x_vector, 2, sf::Lines);

    sf::Vertex velocity_y_vector[] =
    {
        sf::Vertex(sf::Vector2f(x_, y_), sf::Color::Green),
        sf::Vertex(sf::Vector2f(x_, y_+velocity_.y), sf::Color::Green)
    };

    renderWindow.draw(velocity_y_vector, 2, sf::Lines);
}

Tank::Tank(uint32_t id, float x, float y, float rotation, 
        std::unique_ptr<Cannon> cannon, sf::Texture& tankBody)
: RigidBody(id, x, y, TANK_RADIUS, TANK_MASS, GROUND_DRAG_COEEF)
, id_(id)
, cannon_(std::move(cannon))
, current_direction_(rotation)
, set_direction_(rotation)
{
    sprite_.setTexture(tankBody);
    sf::Vector2u texture_body_size = tankBody.getSize();
    tank_middle_point_ = sf::Vector2f(texture_body_size.x / 2, texture_body_size.y / 2);
    sprite_.setOrigin(tank_middle_point_);   
}

void Tank::draw(sf::RenderWindow& renderWindow)
{
    sprite_.setColor(sf::Color(10, 10, 10, 127));
    sprite_.setPosition(x_ + 8, y_+ 8);
    sprite_.setRotation(current_direction_ - 90.f);
    renderWindow.draw(sprite_);

    sprite_.setColor(sf::Color(255, 255, 255, 255));
    sprite_.setPosition(x_, y_);
    renderWindow.draw(sprite_);
    cannon_->x_ = x_;
    cannon_->y_ = y_;
    cannon_->draw(renderWindow);

    if(DEBUG_) drawDebugInfo(renderWindow);

    sf::Vector2f left_track = math::rotate_point(sf::Vector2f(x_, y_-15.0), current_direction_, sf::Vector2f(x_, y_));
    sf::Vector2f right_track = math::rotate_point(sf::Vector2f(x_, y_+15.0), current_direction_, sf::Vector2f(x_, y_));

    Context::getParticles().addParticle(left_track.x, left_track.y, current_direction_);
    Context::getParticles().addParticle(right_track.x, right_track.y, current_direction_);
}

void Tank::setThrottle(float throttle)
{
    set_throttle_ = throttle;
}
void Tank::setDirection(float direction)
{
    set_direction_ = direction;
    cannon_->setRotation(direction);
}

void Tank::onPhysics(std::vector<std::unique_ptr<RigidBody>>& objects, float timeStep)
{
    // TODO Tanks now does not use rotational speed for rotation 
    // so to other collisions to work it is important to not pass fake angular velociy
    // on contacts
    angular_velocity_ = 0;

    (void) objects;
    //Convert current direction to 0..360 range
    current_direction_ = math::signed_fmod(current_direction_, 360.0);

    float delta = set_direction_ - current_direction_;
    delta = math::signed_fmod((delta + 180.0), 360.0) - 180.0;
    // If current direction of movement is different(more than 15deg) than current one cut the throttle
    if (fabs(delta) > 15.0) current_throttle_ = 0.0; else current_throttle_ = set_throttle_;
    if (delta > 0.0) current_direction_+= std::min(TANK_ROTATION_SPEED* timeStep, std::fabs(delta)) ;
    if (delta < 0.0) current_direction_-= std::min(TANK_ROTATION_SPEED* timeStep, std::fabs(delta)) ;

    //TODO add some inertia calculation while accelerating
    drivetrain_force_.x = cos(current_direction_ * M_PI/180.0) * (current_throttle_ * TANK_ACCELERATION);
    drivetrain_force_.y = sin(current_direction_ * M_PI/180.0) * (current_throttle_ * TANK_ACCELERATION);

    braking_force_.x = -velocity_.x * TANK_BRAKE_FORCE *(1.0 - current_throttle_);
    braking_force_.y = -velocity_.y * TANK_BRAKE_FORCE *(1.0 - current_throttle_);

    applyForce(drivetrain_force_ + braking_force_);

    cannon_->physics(timeStep);
}

}  // namespace game
