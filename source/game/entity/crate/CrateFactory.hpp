#pragma once


#include <memory>

namespace game::entity { class Crate; }

namespace game::entity 
{

class CrateFactory
{
public:
    enum class CrateType
    {
        Wood,
        Metal
    };

    static std::unique_ptr<Crate> create(const CrateFactory::CrateType type, float x, float y);
};

}  // namespace game::entity

