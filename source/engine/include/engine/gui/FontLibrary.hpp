#pragma once

#include <map>
#include <string>
#include <string_view>

#include "engine/gfx/Font.hpp"


namespace engine::gui
{

class FontLibrary
{
public:
    FontLibrary() = delete;
    static void init();
    static engine::gfx::Font& get(const std::string_view& name);
    static void destroy();

private:
    static void initDefaultFont();
    static void initFont(const std::string_view& name, const std::string_view& path);
    static std::map<std::string, engine::gfx::Font> fonts_;
};

}  // namespace engine::gui
