#ifndef GAME_CRATE_HPP_
#define GAME_CRATE_HPP_

#include <SFML/Graphics.hpp>

#include "game/GameObject.hpp"

namespace game
{


// TODO: Crate has now only circle collision as this is only type of colission
// supported by RigidBody. I need to figure out AABB or OBB collisions
// to support more complex convex shapes ( like box 🤣 )
// Need to check SAT theorem which migh help solving that

class Crate : public GameObject
{
public:
    Crate(float x, float y, sf::Texture& barrelBody);
    virtual ~Crate() = default;
};

}  // namespace game

#endif  // GAME_CRATE_HPP_