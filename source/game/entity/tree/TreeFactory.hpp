#pragma once


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

