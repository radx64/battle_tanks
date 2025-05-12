#pragma once

#include <cstddef>
#include <vector>

#include "engine/Logger.hpp"
#include "gui/layout/Constraint.hpp"

namespace gui::layout
{

/*
    Precedence of size constraints:
        1. Fixed size in pixels
        2. Fixed size in percentage
        3. Auto size
 */

class Element
{
public:
    Element();
    Element(const Constraint& constraint);

    Constraint constraint;
    float resolvedWidth;
};

class SizeConstraintResolver
{
public:
    SizeConstraintResolver(const std::string_view logPrefix);

    void setTotalSize(const float size);
    void setElementCount(const size_t count);
    void setElementSize(const size_t index, const Constraint& constraint);
    void resetElement(const size_t index);
    void addElementAtIndex(const size_t index, const Constraint& constraint);
    void removeElementAtIndex(const size_t index);
    float getElementSize(const size_t index) const;
    size_t getElementsCount() const;

protected:
    void resolveElementsSizes();

    std::vector<Element> elements_;
    float size_;
    engine::Logger logger_;
};

}  // namespace gui::layout
