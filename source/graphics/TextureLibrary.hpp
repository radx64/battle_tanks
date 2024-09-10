#pragma once

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

namespace graphics
{

class TextureLibrary
{
public:
    TextureLibrary() = delete;
    static void initialize();
    static sf::Texture& get(const std::string& name);
    static void destroy();

private:
    static void initTexture(const std::string& name, const std::string& path);
    static std::map<std::string, sf::Texture> textures_;
};

}  // namespace graphics
