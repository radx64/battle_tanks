#include "FramedSprite.hpp"

namespace
{
    constexpr auto NUMBER_OF_QUADS = 9;
    constexpr auto NUMBER_OF_VERTICES_PER_QUAD = 4;
    constexpr auto NUMBER_OF_ALL_VERTICES = NUMBER_OF_QUADS * NUMBER_OF_VERTICES_PER_QUAD;
}  // namespace

namespace gui
{

FramedSprite::FramedSprite(const LayoutConfig& layoutConfig)
    : layoutConfig_(layoutConfig)
    , texture_{nullptr}
{
    verticies_.setPrimitiveType(sf::Quads);
    verticies_.resize(NUMBER_OF_ALL_VERTICES);
}

void FramedSprite::setSize(const sf::Vector2f& size)
{
    size_ = size;
    recalculateVerticies();
}

const sf::Vector2f& FramedSprite::getSize() const
{
    return size_;
}

void FramedSprite::setTexture(const sf::Texture& texture)
{
    texture_ = &texture;
}

const sf::Texture& FramedSprite::getTexture() const
{
    return *texture_;
}

void FramedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform.combine(getTransform());
    states.texture = texture_;
    target.draw(verticies_, states);
}

void FramedSprite::recalculateVerticies()
{
    if (not texture_) return;

    const auto& cornerSizes = layoutConfig_.cornerSizes;
    const auto& uvs = layoutConfig_.uvs;

    setQuad(0, {0.f, 0.f, cornerSizes.topLeft.x, cornerSizes.topLeft.y}, uvs.topLeft);
    setQuad(1, {cornerSizes.topLeft.x, 0.f, size_.x - cornerSizes.topLeft.x - cornerSizes.bottomRight.x, cornerSizes.topLeft.y}, {uvs.topLeft.left + uvs.topLeft.width, uvs.topLeft.top, uvs.topRight.width, uvs.topRight.height});
    setQuad(2, {size_.x - cornerSizes.bottomRight.x, 0.f, cornerSizes.bottomRight.x, cornerSizes.topLeft.y}, uvs.topRight);
    setQuad(3, {0.f, cornerSizes.topLeft.y, cornerSizes.topLeft.x, size_.y - cornerSizes.topLeft.y - cornerSizes.bottomRight.y}, {uvs.topLeft.left, uvs.topLeft.top + uvs.topLeft.height, uvs.bottomLeft.width, uvs.bottomLeft.height});
    setQuad(4, {cornerSizes.topLeft.x, cornerSizes.topLeft.y, size_.x - cornerSizes.topLeft.x - cornerSizes.bottomRight.x, size_.y - cornerSizes.topLeft.y - cornerSizes.bottomRight.y}, {uvs.topLeft.left + uvs.topLeft.width, uvs.topLeft.top + uvs.topLeft.height, uvs.topRight.width, uvs.topRight.height});
    setQuad(5, {size_.x - cornerSizes.bottomRight.x, cornerSizes.topLeft.y, cornerSizes.bottomRight.x, size_.y - cornerSizes.topLeft.y - cornerSizes.bottomRight.y}, {uvs.topRight.left, uvs.topRight.top + uvs.topRight.height, uvs.bottomRight.width, uvs.bottomRight.height});
    setQuad(6, {0.f, size_.y - cornerSizes.bottomRight.y, cornerSizes.topLeft.x, cornerSizes.bottomRight.y}, uvs.bottomLeft);
    setQuad(7, {cornerSizes.topLeft.x, size_.y - cornerSizes.bottomRight.y, size_.x - cornerSizes.topLeft.x - cornerSizes.bottomRight.x, cornerSizes.bottomRight.y}, {uvs.bottomLeft.left + uvs.bottomLeft.width, uvs.bottomLeft.top, uvs.bottomRight.width, uvs.bottomRight.height});
    setQuad(8, {size_.x - cornerSizes.bottomRight.x, size_.y - cornerSizes.bottomRight.y, cornerSizes.bottomRight.x, cornerSizes.bottomRight.y}, uvs.bottomRight);
}

void FramedSprite::setVertex(size_t index, const sf::Vector2f& position, const sf::Vector2f& uv)
{
    verticies_[index].position = position;
    verticies_[index].texCoords = uv;
}

void FramedSprite::setQuad(size_t index, const sf::FloatRect& rect, const sf::FloatRect& uv)
{
    setVertex(NUMBER_OF_VERTICES_PER_QUAD * index + 0, {rect.left, rect.top}, {uv.left, uv.top});
    setVertex(NUMBER_OF_VERTICES_PER_QUAD * index + 1, {rect.left + rect.width, rect.top}, {uv.left + uv.width, uv.top});
    setVertex(NUMBER_OF_VERTICES_PER_QUAD * index + 2, {rect.left + rect.width, rect.top + rect.height}, {uv.left + uv.width, uv.top + uv.height});
    setVertex(NUMBER_OF_VERTICES_PER_QUAD * index + 3, {rect.left, rect.top + rect.height}, {uv.left, uv.top + uv.height});
}

} // namespace gui
