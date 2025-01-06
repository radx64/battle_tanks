#pragma once

#include <string_view>

#include <chrono>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

namespace engine
{

class Logger
{
public:
    Logger() = delete;

    static void error(const std::string_view log);
    static void warning(const std::string_view log);
    static void info(const std::string_view log);
    static void debug(const std::string_view log);

protected:
    static void print(const fmt::v9::color color, const std::string_view logType, const std::string_view& log);
};

}  // namespace engine 
