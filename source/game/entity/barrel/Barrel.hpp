#pragma once


#include "engine/GameObject.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity
{

class Barrel : public engine::GameObject
{
public:
    Barrel(float x, float y, engine::gfx::Texture& barrelBody);
    virtual ~Barrel() = default;
};



}  // namespace game::entity

