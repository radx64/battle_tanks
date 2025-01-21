#include "gui/DimensionConstraintScaler.hpp"

#include <algorithm>

#include <fmt/format.h>

namespace gui
{

DimensionConstraintScaler::DimensionConstraintScaler(const std::string_view logPrefix)
: logger_{std::string(logPrefix) + "/DimensionConstraintScaler"}
{
}

void DimensionConstraintScaler::setSize(const float size)
{
    size_ = size;
}

void DimensionConstraintScaler::setElementCount(const size_t count)
{
    elements_.resize(count);
}

void DimensionConstraintScaler::setElementSize(const size_t index, const float ratio)
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("setElementSize: Index {} is out of bounds", index));
        return;
    }
    elements_[index] = ratio;
}

void DimensionConstraintScaler::clearElementSize(const size_t index)
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("clearElementSize: Index {} is out of bounds", index));
        return;
    }
    elements_[index] = std::nullopt;
}

void DimensionConstraintScaler::addElementAtIndex(const size_t index)
{
    if (index > elements_.size())
    {
        logger_.error(fmt::format("addElementAtIndex: Index {} is out of bounds", index));
        return;
    }
    elements_.insert(std::begin(elements_) + index, std::nullopt);
}

void DimensionConstraintScaler::removeElementAtIndex(const size_t index)
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("removeElementAtIndex: Index {} is out of bounds", index));
        return;
    }
    elements_.erase(std::begin(elements_) + index);
}

float DimensionConstraintScaler::getElementSize(const size_t index) const
{
    if (index >= elements_.size())
    {
        logger_.error(fmt::format("getElementSize: Index {} is out of bounds", index));
        return 0.f;
    }

    auto elementSize = elements_[index];

    if (elementSize.has_value())
    {
        return elementSize.value() * size_;
    }
    else
    {
        //return average size of empty elements
        auto emptyElements = std::count(std::cbegin(elements_), std::cend(elements_), std::nullopt);
        // there should not be zero empty elements as if element has no value there should be at least one

        float usedRatioSum = 0.f;
        for (const auto& element : elements_)
        {
            if (element.has_value())
            {
                usedRatioSum += element.value();
            }
        }

        float remainingSize = size_ * (1.f - usedRatioSum);
        return remainingSize / emptyElements;
    }
}

}  // namespace gui
