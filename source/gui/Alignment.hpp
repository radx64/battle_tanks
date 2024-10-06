#pragma once

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

Alignment operator|(Alignment lhs, Alignment rhs);
Alignment operator&(Alignment lhs, Alignment rhs);
bool isBitSet(const Alignment& alignment, const Alignment& bit);

}  // namespace gui
