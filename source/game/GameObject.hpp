#ifndef GAME_GAME_OBJECT_HPP_
#define GAME_GAME_OBJECT_HPP_

namespace game {class World;}
namespace game {class Renderer;}
namespace sf {class RenderWindow;}

namespace game
{

class GameObject
{
public:

    void draw(sf::RenderWindow& renderWindow, float timeStep)
    {
        (void) timeStep;
        if (renderer_)  renderer_->draw(renderWindow); // TODO add timeStep 
    }
    void update(game::World& world, float timeStep)
    {
        (void) world;
        (void) timeStep;     
    }


    std::unique_ptr<Renderer> renderer_;
};


} // namespace game


#endif  // GAME_GAME_OBJECT_HPP_