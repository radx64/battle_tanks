#pragma once

#include <SFML/Graphics.hpp>

// TODO: add fwds when moving to cpp

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

constexpr auto NUMBER_OF_QUADS = 9;
constexpr auto NUMBER_OF_VERTICES_PER_QUAD = 4;
constexpr auto NUMBER_OF_ALL_VERTICES = NUMBER_OF_QUADS * NUMBER_OF_VERTICES_PER_QUAD;

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

        CornersSizes cornerSizes;
        UVs uvs;
    };

    FramedSprite(const LayoutConfig& layoutConfig)
        : layoutConfig_(layoutConfig)
        , texture_{nullptr}
    {
        verticies_.setPrimitiveType(sf::Quads);
        verticies_.resize(NUMBER_OF_ALL_VERTICES);
    }

    void setSize(const sf::Vector2f& size)
    {
        size_ = size;
        recalculateVerticies();
    }

    const sf::Vector2f& getSize() const
    {
        return size_;
    }
    
    void setTexture(const sf::Texture& texture)
    {
        texture_ = &texture;
    }
    const sf::Texture& getTexture() const
    {
        return *texture_;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform.combine(getTransform());
        states.texture = texture_;
        target.draw(verticies_, states);
    }

    void recalculateVerticies()
    {
        if (not texture_) return;

        //const auto textureSize = texture_->getSize();
        const auto& cornerSizes = layoutConfig_.cornerSizes;
        const auto& uvs = layoutConfig_.uvs;

        auto setVertex = [this](size_t index, const sf::Vector2f& position, const sf::Vector2f& uv)
        {
            verticies_[index].position = position;
            verticies_[index].texCoords = uv;
        };

        auto setQuad = [this, &setVertex](size_t index, const sf::FloatRect& rect, const sf::FloatRect& uv)
        {
            setVertex(index, {rect.left, rect.top}, {uv.left, uv.top});
            setVertex(index + 1, {rect.left + rect.width, rect.top}, {uv.left + uv.width, uv.top});
            setVertex(index + 2, {rect.left + rect.width, rect.top + rect.height}, {uv.left + uv.width, uv.top + uv.height});
            setVertex(index + 3, {rect.left, rect.top + rect.height}, {uv.left, uv.top + uv.height});
        };


        setQuad(0, {0.f, 0.f, cornerSizes.topLeft.x, cornerSizes.topLeft.y}, uvs.topLeft);
        setQuad(4, {cornerSizes.topLeft.x, 0.f, size_.x - cornerSizes.topLeft.x - cornerSizes.bottomRight.x, cornerSizes.topLeft.y}, {uvs.topLeft.left + uvs.topLeft.width, uvs.topLeft.top, uvs.topRight.width, uvs.topRight.height});
        setQuad(8, {size_.x - cornerSizes.bottomRight.x, 0.f, cornerSizes.bottomRight.x, cornerSizes.topLeft.y}, uvs.topRight);
        setQuad(12, {0.f, cornerSizes.topLeft.y, cornerSizes.topLeft.x, size_.y - cornerSizes.topLeft.y - cornerSizes.bottomRight.y}, {uvs.topLeft.left, uvs.topLeft.top + uvs.topLeft.height, uvs.bottomLeft.width, uvs.bottomLeft.height});
        setQuad(16, {cornerSizes.topLeft.x, cornerSizes.topLeft.y, size_.x - cornerSizes.topLeft.x - cornerSizes.bottomRight.x, size_.y - cornerSizes.topLeft.y - cornerSizes.bottomRight.y}, {uvs.topLeft.left + uvs.topLeft.width, uvs.topLeft.top + uvs.topLeft.height, uvs.topRight.width, uvs.topRight.height});
        setQuad(20, {size_.x - cornerSizes.bottomRight.x, cornerSizes.topLeft.y, cornerSizes.bottomRight.x, size_.y - cornerSizes.topLeft.y - cornerSizes.bottomRight.y}, {uvs.topRight.left, uvs.topRight.top + uvs.topRight.height, uvs.bottomRight.width, uvs.bottomRight.height});
        setQuad(24, {0.f, size_.y - cornerSizes.bottomRight.y, cornerSizes.topLeft.x, cornerSizes.bottomRight.y}, uvs.bottomLeft);
        setQuad(28, {cornerSizes.topLeft.x, size_.y - cornerSizes.bottomRight.y, size_.x - cornerSizes.topLeft.x - cornerSizes.bottomRight.x, cornerSizes.bottomRight.y}, {uvs.bottomLeft.left + uvs.bottomLeft.width, uvs.bottomLeft.top, uvs.bottomRight.width, uvs.bottomRight.height});
        setQuad(32, {size_.x - cornerSizes.bottomRight.x, size_.y - cornerSizes.bottomRight.y, cornerSizes.bottomRight.x, cornerSizes.bottomRight.y}, uvs.bottomRight);

    }

protected:
    sf::VertexArray verticies_;
    sf::Vector2f size_;
    const LayoutConfig layoutConfig_;
    const sf::Texture* texture_;
};

} // namespace gui
