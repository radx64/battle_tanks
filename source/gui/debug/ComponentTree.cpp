#include "gui/debug/ComponentTree.hpp"

#include "gui/Widget.hpp"

#include <fmt/format.h>

namespace gui::debug
{

void ComponentTree::print(const Widget* widget, const std::string& prefix, bool isLast)
{
    fmt::print("{}{}{}\n", prefix, isLast ? "└── " : "├── ", widget->getId());

    
    std::string childPrefix = prefix + (isLast ? "    " : "│   ");
    
    const bool notHasChildren = widget->getChildren().empty();
    fmt::print("{}{}position: x: {}, y: {}\n", childPrefix, (notHasChildren ? "    " : "│   "), widget->getGlobalPosition().x, widget->getGlobalPosition().y);
    fmt::print("{}{}size: width: {}, height: {}\n", childPrefix, (notHasChildren ? "    " : "│   "), widget->getSize().x, widget->getSize().y);
    fmt::print("{}{}isFocusable: {}\n", childPrefix, (notHasChildren ? "    " : "│   "), widget->isFocusable());
    fmt::print("{}{}isVisible: {}\n", childPrefix, (notHasChildren ? "    " : "│   "), widget->isVisible());
    for (size_t i = 0; i < widget->getChildren().size(); ++i) {
        const bool last = (i == widget->getChildren().size() - 1);
        print(widget->getChildren()[i].get(), childPrefix, last);
    }
}

}  // namespace gui::debug
