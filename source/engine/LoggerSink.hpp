#pragma once

#include <string_view>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

namespace engine
{
    
class LoggerSink
{
protected:
    static void print(const fmt::v9::color color, 
        const std::string_view logType,
        const std::string_view prefix,
        const std::string_view log);

    friend class Logger;
};

}  // namespace engine
