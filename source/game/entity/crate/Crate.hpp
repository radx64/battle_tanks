#pragma once


#include "engine/GameObject.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity
{


// TODO: Crate has now only circle collision as this is only type of colission
// supported by RigidBody. I need to figure out AABB or OBB collisions
// to support more complex convex shapes ( like box 🤣 )
// Need to check SAT theorem which migh help solving that

class Crate : public engine::GameObject
{
public:
    Crate(float x, float y, engine::gfx::Texture& barrelBody);
    virtual ~Crate() = default;
};

}  // namespace game::entity

