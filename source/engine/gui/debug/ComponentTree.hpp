#pragma once

#include <string>

namespace engine::gui { class Widget; }

namespace engine::gui::debug
{
class ComponentTree
{
public:
    static void print(const Widget* widget, const std::string& prefix = "", bool isLast = true);
};
}  // namespace engine::gui::debug
