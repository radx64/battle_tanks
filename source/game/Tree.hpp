#ifndef GAME_TREE_HPP_
#define GAME_TREE_HPP_

#include <SFML/Graphics.hpp>

#include "game/IRenderable.hpp"
#include "game/RigidBody.hpp"

namespace game
{

class Tree : public IRenderable, public RigidBody
{
public:

    enum class Type
    {
        Green_Small,
        Green_Large,
        Brown_Small,
        Brown_Large
    };

    Tree(uint32_t id, float x, float y, const Type& treeType);
    virtual ~Tree() = default;
    void draw(sf::RenderWindow& renderWindow) override;

protected:
    sf::Sprite sprite_;
};

}  // namespace game

#endif  // GAME_TREE_HPP_