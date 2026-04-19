#pragma once

#include <map>
#include <string>
#include <string_view>

#include <SFML/Graphics.hpp>

namespace gui
{

class FontLibrary
{
public:
    FontLibrary() = delete;
    static void init();
    static sf::Font& get(const std::string_view& name);
    static void destroy();

private:
    static void initDefaultFont();
    static void initFont(const std::string_view& name, const std::string_view& path);
    static std::map<std::string, sf::Font> fonts_;
};

}  // namespace gui
