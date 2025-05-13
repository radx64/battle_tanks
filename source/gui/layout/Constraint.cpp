#include "gui/layout/Constraint.hpp"

namespace gui::layout
{
Constraint Constraint::Auto()
{
    return Constraint(Type::Auto, Unit::Percentage, 0.f);
}

Constraint Constraint::Percent(const float value)
{
    return Constraint(Type::Fixed, Unit::Percentage, value);
}

Constraint Constraint::Pixels(const float value)
{
    return Constraint(Type::Fixed, Unit::Pixels, value);
}

Constraint::Type Constraint::getType() const
{
    return type;
}

Constraint::Unit Constraint::getUnit() const
{
    return unit;
}

float Constraint::getValue() const
{
    return value;
}

void Constraint::setType(const Type type)
{
    this->type = type;
}

void Constraint::setUnit(const Unit unit)
{
    this->unit = unit;
}

void Constraint::setValue(const float value)
{
    this->value = value;
}

Constraint::Constraint(const Type type, const Unit unit, const float value)
: type(type)
, unit(unit)
, value(value)
{

}
}  // namespace gui::layout
