#pragma once

#include <string>

namespace gui { class Widget; }

namespace gui::debug
{
class ComponentTree
{
public:
    static void print(const Widget* widget, const std::string& prefix = "", bool isLast = true);
};
}  // namespace gui::debug
