#include "graphics/FontLibrary.hpp"

void FontLibrary::initFont(const std::string& name, const std::string& path)
{
    auto placed_element = fonts_.emplace(std::make_pair(name, sf::Font()));
    bool has_loaded = placed_element.first->second.loadFromFile(path);
    if(not has_loaded) throw std::string("Couldn't load texture file...");
}

void FontLibrary::initialize()
{
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