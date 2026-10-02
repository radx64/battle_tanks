#pragma once


#include "engine/GameObject.hpp"
#include "engine/gfx/Texture.hpp"

namespace game::entity
{

class Tree : public engine::GameObject
{
public:
    Tree(float x, float y, float height, engine::gfx::Texture& treeBody, float treeTrunkRadius);
    virtual ~Tree() = default;

    float height_;
};

}  // namespace game::entity

