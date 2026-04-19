#pragma once

#include <memory>

#include "gui/style/Sheet.hpp"

namespace gui::style
{
class StyleFactory
{
public:
    static const Sheet& instance();

private:
    static std::unique_ptr<Sheet> sheet_;
};

}  // namespace gui::style
