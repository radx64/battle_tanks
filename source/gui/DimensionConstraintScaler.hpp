#pragma once

#include <cstddef>
#include <vector>

#include "engine/Logger.hpp"

namespace gui
{

/*
    Precedence of size constraints:
        1. Fixed size in pixels
        2. Fixed size in percentage
        3. Auto size
 */
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

class Element
{
public:
    Element();
    Element(const SizeConstraint& constraint);

    SizeConstraint constraint;
    float resolvedWidth;
};

class DimensionConstraintScaler
{
public:
    DimensionConstraintScaler(const std::string_view logPrefix);

    void setTotalSize(const float size);
    void setElementCount(const size_t count);
    void setElementSize(const size_t index, const SizeConstraint& constraint);
    void resetElement(const size_t index);
    void addElementAtIndex(const size_t index, const SizeConstraint& constraint);
    void removeElementAtIndex(const size_t index);
    float getElementSize(const size_t index) const;

protected:
    void resolveElementsSizes();

    std::vector<Element> elements_;
    float size_;
    engine::Logger logger_;
};

}  // namespace gui
