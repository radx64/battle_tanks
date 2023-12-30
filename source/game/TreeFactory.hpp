#ifndef GAME_TREE_FACTORY_HPP_
#define GAME_TREE_FACTORY_HPP_

#include <memory>

namespace sf { class Texture; }
namespace game { class Tree; }

namespace game 
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

}  // namespace game

#endif  // GAME_TREE_FACTORY_HPP_