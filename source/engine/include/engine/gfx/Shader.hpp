#pragma once

#include <memory>
#include <string>

#include "engine/Vector2.hpp"

namespace engine::backends::sfml
{
class SfmlRenderTarget;
}

namespace engine::gfx
{

class Shader
{
public:
    enum class Type
    {
        Vertex,
        Fragment
    };

    Shader();
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&&) noexcept;
    Shader& operator=(Shader&&) noexcept;

    static bool isAvailable();

    bool loadFromFile(const std::string& filename, Type type);
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, const engine::Vec2f& value);
    void setUniformCurrentTexture(const std::string& name);

    class Impl;

private:
    friend class engine::backends::sfml::SfmlRenderTarget;

    std::unique_ptr<Impl> impl_;
};

}  // namespace engine::gfx
