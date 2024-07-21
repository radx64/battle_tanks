#pragma once


#include <memory>

namespace game::entity { class Tank; }
namespace game::entity { class TracksRenderer; }

namespace game::entity 
{

class TankFactory
{
public:
    enum class TankType
    {
        Red,
        Green,
        Blue,
        Sand,
        Black
    };

    static std::unique_ptr<Tank> create(const TankFactory::TankType type, float x, float y, float rotation, TracksRenderer* tracksRenderer);
};

}  // namespace game::entity

