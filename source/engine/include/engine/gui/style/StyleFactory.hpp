#pragma once

#include <memory>

#include "engine/gui/style/Sheet.hpp"

namespace engine::gui::style
{
class StyleFactory
{
public:
    static const Sheet& instance();

private:
    static std::unique_ptr<Sheet> sheet_;
};

}  // namespace engine::gui::style
