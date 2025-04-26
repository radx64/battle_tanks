#include "gui/DimensionConstraintScaler.hpp"

#include <fmt/format.h>

namespace gui
{

SizeConstraint SizeConstraint::Auto()
{
    return SizeConstraint(Type::Auto, Unit::Percentage, 0.f);
}

SizeConstraint SizeConstraint::Fixed(const float value, const Unit unit)
{
    return SizeConstraint(Type::Fixed, unit, value);
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

Element::Element()
: constraint(SizeConstraint::Auto())
, resolvedWidth(0.f)
{

}

Element::Element(const SizeConstraint& constraint)
: constraint(constraint)
, resolvedWidth(0.f)
{

}

DimensionConstraintScaler::DimensionConstraintScaler(const std::string_view logPrefix)
: size_{}
, logger_{std::string(logPrefix) + "/DimensionConstraintScaler"}
{
}

void DimensionConstraintScaler::setTotalSize(const float size)
{
    size_ = size;
    resolveElementsSizes();
}

void DimensionConstraintScaler::setElementCount(const size_t count)
{
    elements_.resize(count);
    resolveElementsSizes();
}

void DimensionConstraintScaler::setElementSize(const size_t index, const SizeConstraint& constraint)
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("setElementSize: Index {} is out of bounds", index));
        return;
    }

    elements_[index].constraint = constraint;
    resolveElementsSizes();
}

void DimensionConstraintScaler::resetElement(const size_t index)
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("clearElementSize: Index {} is out of bounds", index));
        return;
    }
    elements_[index].constraint = SizeConstraint::Auto();
    resolveElementsSizes();
}

void DimensionConstraintScaler::addElementAtIndex(const size_t index, const SizeConstraint& constraint)
{
    if (index > elements_.size())
    {
        logger_.error(fmt::format("addElementAtIndex: Index {} is out of bounds", index));
        return;
    }
    elements_.insert(std::begin(elements_) + index, Element(constraint));
    resolveElementsSizes();
}

void DimensionConstraintScaler::removeElementAtIndex(const size_t index)
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("removeElementAtIndex: Index {} is out of bounds", index));
        return;
    }
    elements_.erase(std::begin(elements_) + index);
    resolveElementsSizes();
}

float DimensionConstraintScaler::getElementSize(const size_t index) const
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("getElementSize: Index {} is out of bounds", index));
        return 0.f;
    }

    return elements_[index].resolvedWidth;
}

void DimensionConstraintScaler::resolveElementsSizes()
{
    float totalFixedWidth = 0.f;
    float totalPercentageWidth = 0.f;
    float totalAvailableWidth = size_;
    size_t autoElementCount = 0;

    size_t index = 0;
    for (const auto& element : elements_)
    {
        float value = element.constraint.getValue();

        if (value < 0.f)
        {
            logger_.warning(fmt::format("resolveElementsSizes: Constraint value for element at position {} is negative: {}, clamping to 0.f", index, value));
            value = 0.f;
        }

        value = std::max(0.f, element.constraint.getValue()); // clamp negative values

        if (element.constraint.getType() == SizeConstraint::Type::Fixed)
        {
            if (element.constraint.getUnit() == SizeConstraint::Unit::Pixels)
            {
                totalFixedWidth += element.constraint.getValue();
            }
            else if (element.constraint.getUnit() == SizeConstraint::Unit::Percentage)
            {
                totalPercentageWidth += element.constraint.getValue();
            }
        } 
        else if (element.constraint.getType() == SizeConstraint::Type::Auto)
        {
            autoElementCount++;
        }
   
        ++index;
    }

    float remainingWidth = totalAvailableWidth - totalFixedWidth;

    if (remainingWidth < 0.f) 
    {
        // Not enough space for all fixed components
        remainingWidth = 0.f;
        totalPercentageWidth = 0.f; // Don't allow percentages if space is gone
    }

    float percentageNormalizationFactor = 1.f;

    // Normalize percent widths if they exceed 100%, I'm not doing that for pixels as I am treating them as fixed
    if (totalPercentageWidth > 100.0f) {
         logger_.warning("resolveElementsSizes: Total percentage exceeds 100%, normalizing");
         percentageNormalizationFactor = 100.0f / totalPercentageWidth;
    }

    float totalPercentageWidthInPixels = 0.f;

    // First pass: calculate fixed and percentage widths
    for (auto& element : elements_)
    {
        if (element.constraint.getType() == SizeConstraint::Type::Fixed)
        {
            if (element.constraint.getUnit() == SizeConstraint::Unit::Pixels)
            {
                element.resolvedWidth = element.constraint.getValue();
            }
            else if (element.constraint.getUnit() == SizeConstraint::Unit::Percentage)
            {
                float normalizedValue = element.constraint.getValue() * percentageNormalizationFactor;
                element.resolvedWidth = remainingWidth * (normalizedValue / 100.f);
                totalPercentageWidthInPixels += element.resolvedWidth;
            }
        }
    }

    remainingWidth -= totalPercentageWidthInPixels;

    // Second pass: calculate auto widths
    for (auto& element : elements_)
    {
        if (element.constraint.getType() == SizeConstraint::Type::Auto)
        {
            element.resolvedWidth = remainingWidth / autoElementCount;
        }
    }
}

}  // namespace gui
