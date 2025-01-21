#pragma once

#include <cstddef>
#include <optional>
#include <vector>

#include "engine/Logger.hpp"

namespace gui
{

class DimensionConstraintScaler
{
public:
    DimensionConstraintScaler(const std::string_view logPrefix);

    void setSize(const float size);
    void setElementCount(const size_t count);
    void setElementSize(const size_t index, const float ratio);
    void clearElementSize(const size_t index);
    void addElementAtIndex(const size_t index);
    void removeElementAtIndex(const size_t index);
    float getElementSize(const size_t index) const;

protected:
    std::vector<std::optional<float>> elements_;
    float size_;
    engine::Logger logger_;
};

}  // namespace gui
