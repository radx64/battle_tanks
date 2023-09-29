#include "gui/FontLibrary.hpp"

namespace gui
{

void FontLibrary::initDefaultFont()
{
    fonts_.emplace(std::make_pair("default", sf::Font()));
}

void FontLibrary::initFont(const std::string& name, const std::string& path)
{
    auto placed_element = fonts_.emplace(std::make_pair(name, sf::Font()));
    bool has_loaded = placed_element.first->second.loadFromFile(path);
    if(not has_loaded) throw std::string("Couldn't load font file...");
}

void FontLibrary::initialize()
{
    initDefaultFont();
    initFont("armata", "../fonts/armata.ttf");
    initFont("glassTTY", "../fonts/Glass_TTY_VT220.ttf");
}

sf::Font& FontLibrary::get(const std::string& name)
{
    auto font = fonts_.find(name);
    if (font == fonts_.end())
        throw std::string("No font called " + name + " found in library");
    return font->second;
}

std::map<std::string, sf::Font> FontLibrary::fonts_;

}  // namespace gui
