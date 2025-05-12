#include "gui/layout/SizeConstraintResolver.hpp"

#include <fmt/format.h>

namespace gui::layout
{

Element::Element()
: constraint(Constraint::Auto())
, resolvedWidth(0.f)
{

}

Element::Element(const Constraint& constraint)
: constraint(constraint)
, resolvedWidth(0.f)
{

}

SizeConstraintResolver::SizeConstraintResolver(const std::string_view logPrefix)
: size_{}
, logger_{std::string(logPrefix) + "/SizeConstraintResolver"}
{
}

void SizeConstraintResolver::setTotalSize(const float size)
{
    size_ = size;
    resolveElementsSizes();
}

void SizeConstraintResolver::setElementsCount(const size_t count)
{
    elements_.resize(count);
    resolveElementsSizes();
}

void SizeConstraintResolver::setElementSize(const size_t index, const Constraint& constraint)
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("setElementSize: Index {} is out of bounds", index));
        return;
    }

    elements_[index].constraint = constraint;
    resolveElementsSizes();
}

void SizeConstraintResolver::resetElement(const size_t index)
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("clearElementSize: Index {} is out of bounds", index));
        return;
    }
    elements_[index].constraint = Constraint::Auto();
    resolveElementsSizes();
}

void SizeConstraintResolver::addElementAtIndex(const size_t index, const Constraint& constraint)
{
    if (index > elements_.size())
    {
        logger_.error(fmt::format("addElementAtIndex: Index {} is out of bounds", index));
        return;
    }
    elements_.insert(std::begin(elements_) + index, Element(constraint));
    resolveElementsSizes();
}

void SizeConstraintResolver::removeElementAtIndex(const size_t index)
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("removeElementAtIndex: Index {} is out of bounds", index));
        return;
    }
    elements_.erase(std::begin(elements_) + index);
    resolveElementsSizes();
}

float SizeConstraintResolver::getElementSize(const size_t index) const
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("getElementSize: Index {} is out of bounds", index));
        return 0.f;
    }

    return elements_[index].resolvedWidth;
}

size_t SizeConstraintResolver::getElementsCount() const
{
    return elements_.size();
}

void SizeConstraintResolver::resolveElementsSizes()
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

        if (element.constraint.getType() == Constraint::Type::Fixed)
        {
            if (element.constraint.getUnit() == Constraint::Unit::Pixels)
            {
                totalFixedWidth += element.constraint.getValue();
            }
            else if (element.constraint.getUnit() == Constraint::Unit::Percentage)
            {
                totalPercentageWidth += element.constraint.getValue();
            }
        } 
        else if (element.constraint.getType() == Constraint::Type::Auto)
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
        if (element.constraint.getType() == Constraint::Type::Fixed)
        {
            if (element.constraint.getUnit() == Constraint::Unit::Pixels)
            {
                element.resolvedWidth = element.constraint.getValue();
            }
            else if (element.constraint.getUnit() == Constraint::Unit::Percentage)
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
        if (element.constraint.getType() == Constraint::Type::Auto)
        {
            element.resolvedWidth = remainingWidth / autoElementCount;
        }
    }
}

}  // namespace gui::layout
