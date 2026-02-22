#include "gui/FontLibrary.hpp"

namespace gui
{

void FontLibrary::initDefaultFont()
{
    fonts_.emplace(std::make_pair("default", sf::Font()));
}

void FontLibrary::initFont(const std::string& name, const std::string& path)
{
    auto placedElement = fonts_.emplace(std::make_pair(name, sf::Font()));
    bool hasLoaded = placedElement.first->second.loadFromFile(path);
    if(not hasLoaded) throw std::string("Couldn't load font file...");
}

void FontLibrary::init()
{
    initDefaultFont();
    initFont("armata",      "fonts/armata.ttf");
    initFont("glassTTY",    "fonts/Glass_TTY_VT220.ttf");
    initFont("vt323",       "fonts/vt323-latin-400-normal.ttf");
    initFont("ibmVGA",       "fonts/Px437_IBM_VGA_8x16.ttf");
}

sf::Font& FontLibrary::get(const std::string& name)
{
    auto font = fonts_.find(name);
    if (font == fonts_.end())
        throw std::string("No font called " + name + " found in library");
    return font->second;
}

void FontLibrary::destroy()
{
    fonts_.clear();
}

std::map<std::string, sf::Font> FontLibrary::fonts_;

}  // namespace gui
