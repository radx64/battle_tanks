#pragma once

#include "engine/core/KeyCode.hpp"

#include <string_view>

namespace engine::gui::keyboard
{

    std::string_view keyToString(engine::core::KeyCode key);

}  // namespace engine::gui::keyboard
