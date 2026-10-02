#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/Rect.hpp"
#include "engine/Vector2.hpp"
#include "engine/gfx/Color.hpp"
#include "engine/gfx/VertexArray.hpp"
#include "engine/gfx/View.hpp"

namespace engine
{

template <typename T>
sf::Vector2<T> toSf(const engine::Vector2<T>& vector)
{
    return sf::Vector2<T>(static_cast<T>(vector.x), static_cast<T>(vector.y));
}

template <typename T>
engine::Vector2<T> fromSf(const sf::Vector2<T>& vector)
{
    return engine::Vector2<T>(static_cast<T>(vector.x), static_cast<T>(vector.y));
}

inline sf::Color toSf(const engine::gfx::Color& color)
{
    return sf::Color{color.r, color.g, color.b, color.a};
}

inline sf::IntRect toSf(const engine::IntRect& rect)
{
    return sf::IntRect{rect.left, rect.top, rect.width, rect.height};
}

inline sf::FloatRect toSf(const engine::FloatRect& rect)
{
    return sf::FloatRect{rect.left, rect.top, rect.width, rect.height};
}

inline engine::FloatRect fromSf(const sf::FloatRect& rect)
{
    return engine::FloatRect{rect.left, rect.top, rect.width, rect.height};
}

inline sf::PrimitiveType toSf(const engine::gfx::PrimitiveType type)
{
    switch (type)
    {
        case engine::gfx::PrimitiveType::Points: return sf::Points;
        case engine::gfx::PrimitiveType::Lines: return sf::Lines;
        case engine::gfx::PrimitiveType::LineStrip: return sf::LineStrip;
        case engine::gfx::PrimitiveType::Triangles: return sf::Triangles;
        case engine::gfx::PrimitiveType::TriangleStrip: return sf::TriangleStrip;
        case engine::gfx::PrimitiveType::TriangleFan: return sf::TriangleFan;
        case engine::gfx::PrimitiveType::Quads: return sf::Quads;
    }

    return sf::Points;
}

inline sf::View toSf(const engine::gfx::View& view)
{
    sf::View sfView;
    sfView.setCenter(view.getCenter().x, view.getCenter().y);
    sfView.setSize(view.getSize().x, view.getSize().y);
    return sfView;
}

inline engine::gfx::View fromSf(const sf::View& view)
{
    const auto center = view.getCenter();
    const auto size = view.getSize();
    return engine::gfx::View{{center.x, center.y}, {size.x, size.y}};
}

}  // namespace engine
