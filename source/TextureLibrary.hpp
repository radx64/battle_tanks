#ifndef TEXTURE_LIBRARY_HPP_
#define TEXTURE_LIBRARY_HPP_

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

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

#endif  //  TEXTURE_LIBRARY_HPP_