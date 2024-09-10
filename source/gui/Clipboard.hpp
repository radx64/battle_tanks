#pragma once

#include <string>

namespace gui
{

class Clipboard
{
public:
    Clipboard() = delete;

    static void save(const std::string& text);
    static const std::string& retreive();

    static std::string text_;
};

}  // namespace gui
