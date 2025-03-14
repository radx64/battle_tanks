#pragma once

#include <SFML/Graphics.hpp>

namespace engine
{

template <typename AssetsList>
class TextureLibrary
{
public:
    static TextureLibrary& instance();

    // This can be used to force the initialization of the library
    // when lazy initialization is not prefered
    static void init();
    sf::Texture& get(const std::string& name);

protected:
    TextureLibrary();
    void initTexture(const std::string& name, const std::string& path);

    std::map<std::string, sf::Texture> textures_;
};

template <typename AssetsList>
TextureLibrary<AssetsList>& TextureLibrary<AssetsList>::instance()
{
    static TextureLibrary instance;
    return instance;
}

template <typename AssetsList>
void TextureLibrary<AssetsList>::init()
{
    (void) instance();
}

template <typename AssetsList>
sf::Texture& TextureLibrary<AssetsList>::get(const std::string& name)
{
    auto texture = textures_.find(name);
    if (texture == textures_.end())
    {
        throw std::runtime_error("No texture called " + name + " found in library");
    }
    return texture->second;
}
template <typename AssetsList>
TextureLibrary<AssetsList>::TextureLibrary()
{
    const auto& assets = AssetsList::getAssets();

    for(const auto& asset : assets)
    {
        initTexture(asset.first, asset.second);
    }
}
template <typename AssetsList>
void TextureLibrary<AssetsList>::initTexture(const std::string& name, const std::string& path)
{
    auto placedElement = textures_.emplace(std::make_pair(name, sf::Texture()));
    bool hasLoaded = placedElement.first->second.loadFromFile(path);
    if(not hasLoaded) throw std::runtime_error("Couldn't load texture file...");
}

}  // namespace engine
