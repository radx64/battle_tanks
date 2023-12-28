#ifndef GAME_CRATE_FACTORY_HPP_
#define GAME_CRATE_FACTORY_HPP_

#include <memory>

namespace game { class Crate; }

namespace game 
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

protected:
    static uint32_t next_crate_id;
};

}  // namespace game

#endif  // GAME_CRATE_FACTORY_HPP_