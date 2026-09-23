#pragma once

#include <memory>
#include <string>

#include "engine/Vector2.hpp"

namespace engine::backends::sfml
{

class SfmlRenderTarget;

}  // namespace engine::backends::sfml

namespace engine::gfx
{

class RenderTexture;
class Sprite;

class Texture
{
public:
    class Impl;

    Texture();
    ~Texture();

    Texture(const Texture&) = default;
    Texture& operator=(const Texture&) = default;
    Texture(Texture&&) noexcept = default;
    Texture& operator=(Texture&&) noexcept = default;

    bool loadFromFile(const std::string& filename);

    engine::Vec2u getSize() const;

private:
    explicit Texture(std::shared_ptr<Impl> impl);

    friend class RenderTexture;
    friend class Sprite;
    friend class engine::backends::sfml::SfmlRenderTarget;

    std::shared_ptr<Impl> impl_;
};

} // namespace engine::gfx
