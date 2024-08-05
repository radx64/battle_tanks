#pragma once

#include <memory>

namespace game::entity { class Barrel; }

namespace game::entity 
{

class BarrelFactory
{
public:
    enum class BarrelType
    {
        Red,
        Rust,
        Green,
        Black
    };

    static std::unique_ptr<Barrel> create(const BarrelFactory::BarrelType type, float x, float y);
};

}  // namespace game::entity

