#pragma once

#include <string>

namespace gui { class Component; }

namespace gui::debug
{
class ComponentTree
{
public:
    static void print(const Component* component, const std::string& prefix = "", bool isLast = true);
};
}  // namespace gui::debug
