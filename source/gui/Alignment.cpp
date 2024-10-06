#include <gui/Alignment.hpp>

namespace gui
{

Alignment operator|(Alignment lhs, Alignment rhs) 
{
    return static_cast<Alignment>(static_cast<std::underlying_type_t<Alignment>>(lhs) |
        static_cast<std::underlying_type_t<Alignment>>(rhs));
}

Alignment operator&(Alignment lhs, Alignment rhs) 
{
    return static_cast<Alignment>(static_cast<std::underlying_type_t<Alignment>>(lhs) &
        static_cast<std::underlying_type_t<Alignment>>(rhs));
}

bool isBitSet(const Alignment& alignment, const Alignment& bit)
{
    return ((alignment & bit) == bit);
}

}  // namespace gui
