#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include <gui/Component.hpp>

namespace gui
{

class Image : public Component
{
public:
    static std::unique_ptr<Image> create(const sf::Texture& texture);

    void onRender(sf::RenderTexture& renderTexture) override;
    void setTexture(const sf::Texture& texture);

protected:
    Image(const sf::Texture& texture);

    void onPositionChange() override;
    void onSizeChange() override;

    sf::Sprite sprite_;
    const sf::Texture& texture_;
};

}  // namespace gui
