#include "gui/debug/ComponentTree.hpp"

#include "gui/Component.hpp"

#include <fmt/format.h>

namespace gui::debug
{

void ComponentTree::print(const Component* component, const std::string& prefix, bool isLast)
{
    fmt::print("{}{}{}\n", prefix, isLast ? "└── " : "├── ", component->getId());

    
    std::string childPrefix = prefix + (isLast ? "    " : "│   ");
    
    const bool notHasChildren = component->getChildren().empty();
    fmt::print("{}{}position: x: {}, y: {}\n", childPrefix, (notHasChildren ? "    " : "│   "), component->getGlobalPosition().x, component->getGlobalPosition().y);
    fmt::print("{}{}size: width: {}, height: {}\n", childPrefix, (notHasChildren ? "    " : "│   "), component->getSize().x, component->getSize().y);
    fmt::print("{}{}isFocusable: {}\n", childPrefix, (notHasChildren ? "    " : "│   "), component->isFocusable());
    fmt::print("{}{}isVisible: {}\n", childPrefix, (notHasChildren ? "    " : "│   "), component->isVisible());
    for (size_t i = 0; i < component->getChildren().size(); ++i) {
        const bool last = (i == component->getChildren().size() - 1);
        print(component->getChildren()[i].get(), childPrefix, last);
    }
}

}  // namespace gui::debug
