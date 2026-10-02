#include "engine/gfx/Shader.hpp"

#include <SFML/Graphics/Glsl.hpp>

#include "ShaderImpl.hpp"

namespace engine::gfx
{

Shader::Shader()
    : impl_(std::make_unique<Impl>())
{
}

Shader::~Shader() = default;

Shader::Shader(Shader&&) noexcept = default;

Shader& Shader::operator=(Shader&&) noexcept = default;

bool Shader::isAvailable()
{
    return sf::Shader::isAvailable();
}

bool Shader::loadFromFile(const std::string& filename, const Type type)
{
    const auto sfmlType = type == Type::Vertex ? sf::Shader::Vertex : sf::Shader::Fragment;
    return impl_->shader_.loadFromFile(filename, sfmlType);
}

void Shader::setUniform(const std::string& name, const float value)
{
    impl_->shader_.setUniform(name, value);
}

void Shader::setUniform(const std::string& name, const engine::Vec2f& value)
{
    impl_->shader_.setUniform(name, sf::Glsl::Vec2{value.x, value.y});
}

void Shader::setUniformCurrentTexture(const std::string& name)
{
    impl_->shader_.setUniform(name, sf::Shader::CurrentTexture);
}

}  // namespace engine::gfx
