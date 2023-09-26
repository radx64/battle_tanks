#ifndef GRAPHICS_TEXTURE_LIBRARY_HPP_
#define GRAPHICS_TEXTURE_LIBRARY_HPP_

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

private:
    static void initTexture(const std::string& name, const std::string& path);
    static std::map<std::string, sf::Texture> textures_;
};

}  // namespace graphics

#endif  // GRAPHICS_TEXTURE_LIBRARY_HPP_