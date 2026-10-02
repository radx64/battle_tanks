#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace engine::gfx
{

class Font
{
public:
    Font();
    ~Font();

    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
    Font(Font&&) noexcept;
    Font& operator=(Font&&) noexcept;

    bool loadFromFile(const std::string& filename);

    float getLineSpacing(unsigned int characterSize) const;
    float getKerning(std::uint32_t first, std::uint32_t second, unsigned int characterSize) const;
    float getGlyphAdvance(std::uint32_t codepoint, unsigned int characterSize) const;

    class Impl;

private:
    friend class Text;

    std::unique_ptr<Impl> impl_;
};

}  // namespace engine::gfx
