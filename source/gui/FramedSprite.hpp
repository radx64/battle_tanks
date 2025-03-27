#pragma once

#include <SFML/Graphics.hpp>

/*

FramedSprite helps drawing ui sprites
slicing them into 9 pieces. When resized
it will keep the corners and stretch the
middle parts.

+---+-------------------+---+
|TL |        TM         |TR |
+---+-------------------+---+
|   |                   |   |
|   |                   |   |
|   |                   |   |
|LM |      CENTER       |RM |
|   |                   |   |
|   |                   |   |
|   |                   |   |
+---+-------------------+---+
|BL |        BM         |BR |
+---+-------------------+---+
Center and middle parts are dynamically
calculated based on corner sizes.

Above layout is then transformed to UVs
coordinates on the texture, for each vertex.
*/


namespace gui
{

class FramedSprite : public sf::Drawable, public sf::Transformable
{
public:

    struct LayoutConfig
    {
        struct CornersSizes
        {
            sf::Vector2f topLeft;
            sf::Vector2f bottomRight;
        };
        struct UVs
        {
            sf::FloatRect topLeft;
            sf::FloatRect topRight;
            sf::FloatRect bottomLeft;
            sf::FloatRect bottomRight;
        };

        CornersSizes cornerSizes; // corner size in pixels
        UVs uvs; // UVs for each corner size in pixels
    };

    FramedSprite(const LayoutConfig& layoutConfig);
    void setSize(const sf::Vector2f& size);
    const sf::Vector2f& getSize() const;
    void setTexture(const sf::Texture& texture);
    const sf::Texture& getTexture() const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void recalculateVerticies();

protected:
    void setVertex(size_t index, const sf::Vector2f& position, const sf::Vector2f& uv);
    void setQuad(size_t index, const sf::FloatRect& rect, const sf::FloatRect& uv);

    sf::VertexArray verticies_;
    sf::Vector2f size_;
    const LayoutConfig layoutConfig_;
    const sf::Texture* texture_;
};

} // namespace gui
