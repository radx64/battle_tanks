#pragma once

namespace gui::layout
{

class SizeConstraint
{
public:
    enum class Unit
    {
        Pixels,
        Percentage,
    };

    enum class Type
    {
        Fixed,
        Auto,
    };

    static SizeConstraint Auto();
    static SizeConstraint Fixed(const float value, const Unit unit);
    static SizeConstraint Percent(const float value);
    static SizeConstraint Pixels(const float value);

    Type getType() const;
    Unit getUnit() const;
    float getValue() const;

protected:
    SizeConstraint(const Type type, const Unit unit, const float value);

    Type type;
    Unit unit;
    float value;
};

}  // namespace gui::layout
