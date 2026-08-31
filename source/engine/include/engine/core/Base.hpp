#pragma once

#include <cstdint>

namespace engine::core
{

    consteval auto bit(const uint32_t bit)
    {
        return 1 << bit;
    }

}  // namespace engine::core
