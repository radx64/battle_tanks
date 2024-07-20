#ifndef GUI_ALIGNMENT_HPP_
#define GUI_ALIGNMENT_HPP_

#include <type_traits>

namespace gui
{
enum class Alignment
{
    Left                    = 0x01,
    Right                   = 0x02,
    HorizontallyCentered    = 0x04,
    Top                     = 0x10,
    Bottom                  = 0x20,
    VerticallyCentered      = 0x40
};

inline constexpr Alignment operator|(Alignment lhs, Alignment rhs) 
{
    return static_cast<Alignment>(static_cast<std::underlying_type_t<Alignment>>(lhs) |
        static_cast<std::underlying_type_t<Alignment>>(rhs));
}

inline constexpr Alignment operator&(Alignment lhs, Alignment rhs) 
{
    return static_cast<Alignment>(static_cast<std::underlying_type_t<Alignment>>(lhs) &
        static_cast<std::underlying_type_t<Alignment>>(rhs));
}

inline bool isBitSet(const Alignment& alignment, const Alignment& bit)
{
    return ((alignment & bit) == bit);
}

}  // namespace gui

#endif  // GUI_ALIGNMENT_HPP_
