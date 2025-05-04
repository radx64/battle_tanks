#include "gui/layout/SizeConstraint.hpp"

namespace gui::layout
{
SizeConstraint SizeConstraint::Auto()
{
    return SizeConstraint(Type::Auto, Unit::Percentage, 0.f);
}

SizeConstraint SizeConstraint::Percent(const float value)
{
    return SizeConstraint(Type::Fixed, Unit::Percentage, value);
}

SizeConstraint SizeConstraint::Pixels(const float value)
{
    return SizeConstraint(Type::Fixed, Unit::Pixels, value);
}

SizeConstraint::Type SizeConstraint::getType() const
{
    return type;
}

SizeConstraint::Unit SizeConstraint::getUnit() const
{
    return unit;
}

float SizeConstraint::getValue() const
{
    return value;
}

SizeConstraint::SizeConstraint(const Type type, const Unit unit, const float value)
: type(type)
, unit(unit)
, value(value)
{

}
}  // namespace gui::layout
