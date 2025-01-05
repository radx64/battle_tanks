#pragma once

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
    template <typename... Args>
    static void error(Args ... args)
    {
        print(fmt::color::crimson, "ERROR", args...);
    }

    template <typename... Args>
    static void warning(Args ... args)
    {
        print(fmt::color::dark_orange, "WARN", args...);
    }

    template <typename... Args>
    static void info(Args ... args)
    {
        print(fmt::color::light_gray, "INFO", args...);
    }

    template <typename... Args>
    static void debug(Args ... args)
    {
        print(fmt::color::cornflower_blue, "DEBUG", args...);
    }

protected:
    template <typename... Args>
    static void print(const fmt::v9::color color, const std::string_view& logType, const Args&... args)
    {
        const auto date = std::chrono::system_clock::now();  
        const auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(date.time_since_epoch());

        fmt::print( fg(color), "[{:<5}][{:%Y-%m-%d %H:%M:}{:%S}] | {}\n", logType, date, milis, args...);
    }

};

}  // namespace engine 
