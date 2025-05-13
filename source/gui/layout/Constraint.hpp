#pragma once

namespace gui::layout
{

class Constraint
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

    static Constraint Auto();
    static Constraint Percent(const float value);
    static Constraint Pixels(const float value);

    Type getType() const;
    Unit getUnit() const;
    float getValue() const;

    void setType(const Type type);
    void setUnit(const Unit unit);
    void setValue(const float value);

protected:
    Constraint(const Type type, const Unit unit, const float value);

    Type type;
    Unit unit;
    float value;
};

}  // namespace gui::layout
