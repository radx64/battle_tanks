#pragma once

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

namespace gui
{

class FontLibrary
{
public:
    FontLibrary() = delete;
    static void initialize();
    static sf::Font& get(const std::string& name);
    static void destroy();

private:
    static void initDefaultFont();
    static void initFont(const std::string& name, const std::string& path);
    static std::map<std::string, sf::Font> fonts_;
};

}  // namespace gui
