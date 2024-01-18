#ifndef GAME_ENTITY_TREE_FACTORY_HPP_
#define GAME_ENTITY_TREE_FACTORY_HPP_

#include <memory>

namespace sf { class Texture; }
namespace game::entity { class Tree; }

namespace game::entity 
{

class TreeFactory
{
public:
    enum class TreeType
    {
        Green_Small,
        Green_Large,
        Brown_Small,
        Brown_Large
    };

    static std::unique_ptr<Tree> create(const TreeFactory::TreeType type, float x, float y);
};

}  // namespace game::entity

#endif  // GAME_ENTITY_TREE_FACTORY_HPP_