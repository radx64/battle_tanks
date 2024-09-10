#include <gui/Clipboard.hpp>

namespace gui
{

void Clipboard::save(const std::string& text)
{
    Clipboard::text_ = text;
}

const std::string& Clipboard::retreive()
{
    return Clipboard::text_;
}

std::string Clipboard::text_{};

}
