#include "TextureLibrary.hpp"


void TextureLibrary::initTexture(const std::string& name, const std::string& path)
{
    auto placed_element = textures_.emplace(std::make_pair(name, sf::Texture()));
    bool has_loaded = placed_element.first->second.loadFromFile(path);
    if(not has_loaded) throw std::string("Couldn't load texture file...");
}

void TextureLibrary::initialize()
{
    initTexture("blue_tank",    "../sprites/tankBody_blue_outline.png");
    initTexture("blue_cannon",  "../sprites/tankBlue_barrel1_outline.png");
}

sf::Texture& TextureLibrary::get(const std::string& name)
{
    auto texture = textures_.find(name);
    if (texture == textures_.end())
        throw std::string("No texture called " + name + " found in library");

    return texture->second;
}

std::map<std::string, sf::Texture> TextureLibrary::textures_;