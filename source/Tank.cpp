#include "Tank.hpp"

#include <algorithm>
#include <cmath>

#include "Context.hpp"
#include "FontLibrary.hpp"
#include "Math.hpp"
#include "Particles.hpp"
#include "TextureLibrary.hpp"

constexpr double TANK_BRAKE_FORCE = 0.1;
constexpr double TANK_ACCELERATION = 4.0;
constexpr float ROLLING_RESISTANCE_COEEF = 0.98;
constexpr double TANK_ROTATION_SPEED = 200.0;

constexpr double TANK_RADIUS = 25;
constexpr double TANK_MASS = 5;

constexpr bool DEBUG = false;

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

    // Velocity vectors
    sf::Vertex velocity_vector[] =
    {
        sf::Vertex(sf::Vector2f(x_, y_)),
        sf::Vertex(sf::Vector2f(x_, y_)+ (velocity_ * 16.0f))
    };
    renderWindow.draw(velocity_vector, 2, sf::Lines);

    sf::Vertex velocity_x_vector[] =
    {
        sf::Vertex(sf::Vector2f(x_, y_), sf::Color::Red),
        sf::Vertex(sf::Vector2f(x_+velocity_.x * 16.0f, y_), sf::Color::Red)
    };

    renderWindow.draw(velocity_x_vector, 2, sf::Lines);

    sf::Vertex velocity_y_vector[] =
    {
        sf::Vertex(sf::Vector2f(x_, y_), sf::Color::Green),
        sf::Vertex(sf::Vector2f(x_, y_+velocity_.y * 16.0f), sf::Color::Green)
    };

    renderWindow.draw(velocity_y_vector, 2, sf::Lines);
}

Tank::Tank(uint32_t id, double x, double y, double rotation, 
        std::unique_ptr<Cannon> cannon, sf::Texture& tankBody)
: id_(id),
    x_(x),
    y_(y),
    cannon_(std::move(cannon)),
    current_direction_(rotation),
    set_direction_(rotation)
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

    if(DEBUG) drawDebugInfo(renderWindow);

    sf::Vector2f left_track = math::rotate_point(sf::Vector2f(x_, y_-15.0), current_direction_, sf::Vector2f(x_, y_));
    sf::Vector2f right_track = math::rotate_point(sf::Vector2f(x_, y_+15.0), current_direction_, sf::Vector2f(x_, y_));

    Context::getParticles().add_particle(left_track.x, left_track.y, current_direction_);
    Context::getParticles().add_particle(right_track.x, right_track.y, current_direction_);
}

void Tank::set_throtle(double throttle)
{
    set_throttle_ = throttle;
}
void Tank::set_direction(double direction)
{
    set_direction_ = direction;
    cannon_->set_rotation(direction);
}

void Tank::physics(std::vector<std::unique_ptr<Tank>>& tanks, double timeStep)
{
    //Convert current direction to 0..360 range
    current_direction_ = math::signed_fmod(current_direction_, 360.0);

    double delta = set_direction_ - current_direction_;
    delta = math::signed_fmod((delta + 180.0), 360.0) - 180.0;
    // If current direction of movement is different(more than 15deg) than current one cut the throttle
    if (fabs(delta) > 15.0) current_throttle_ = 0.0; else current_throttle_ = set_throttle_;
    if (delta > 0.0) current_direction_+= std::min(TANK_ROTATION_SPEED* timeStep, fabs(delta)) ;
    if (delta < 0.0) current_direction_-= std::min(TANK_ROTATION_SPEED* timeStep, fabs(delta)) ;

    drivetrain_force_.x = cos(current_direction_ * M_PI/180.0) * (current_throttle_ * TANK_ACCELERATION);
    drivetrain_force_.y = sin(current_direction_ * M_PI/180.0) * (current_throttle_ * TANK_ACCELERATION);

    braking_force_.x = -velocity_.x * TANK_BRAKE_FORCE *(1.0 - current_throttle_);
    braking_force_.y = -velocity_.y * TANK_BRAKE_FORCE *(1.0 - current_throttle_);

    velocity_ += drivetrain_force_ + braking_force_ ;
    velocity_ *= ROLLING_RESISTANCE_COEEF;

    cannon_->physics(timeStep);

    //Simple circle collision detection code
    for (auto& other_tank : tanks)
    {
        if (other_tank->id_ == id_) continue; // do not check collision with itself.
        
        double distance_between_tanks = math::distance(x_, y_, other_tank->x_, other_tank->y_);
        double sum_of_radius = TANK_RADIUS*2;

        if (distance_between_tanks <= sum_of_radius)
        { 
            //Collision detected here
            //Calculating hit vectors
            double nx = (other_tank->x_ - x_) / distance_between_tanks;
            double ny = (other_tank->y_ - y_) / distance_between_tanks;
            double p = 2*(velocity_.x * nx + velocity_.y * ny
                - other_tank->velocity_.x * nx - other_tank->velocity_.y * ny) / (TANK_MASS * 2);

            sf::Vector2f tankCollisionVelocity = sf::Vector2f(
                (float)(velocity_.x - p * TANK_MASS * nx),
                (float)(velocity_.y - p * TANK_MASS * ny));
            
            sf::Vector2f otherTankCollisionVelocity = sf::Vector2f(
                (float)(other_tank->velocity_.x + p * TANK_MASS * nx),
                (float)(other_tank->velocity_.y + p * TANK_MASS * ny));

            velocity_ = tankCollisionVelocity;
            other_tank->velocity_ = otherTankCollisionVelocity;

            // Solve static collision to not have on one object on top of each other.
            double tanks_overlap = 0.5f  * (distance_between_tanks - TANK_RADIUS - TANK_RADIUS);
            x_ -= tanks_overlap * (x_ - other_tank->x_) / distance_between_tanks;
            y_ -= tanks_overlap * (y_ - other_tank->y_) / distance_between_tanks;
        }
    };

    x_ += velocity_.x * timeStep;
    y_ += velocity_.y * timeStep;
}
