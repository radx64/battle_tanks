#include "engine/Logger.hpp"

/*
    TODO:
    Implement some log sink + static methods for logging
    Consider moving logging to another thread to not wait block main loop in a future
*/

namespace engine
{

void Logger::error(const std::string_view log)
{
    print(fmt::color::crimson, "ERROR", log);
}

void Logger::warning(const std::string_view log)
{
    print(fmt::color::dark_orange, "WARN", log);
}

void Logger::info(const std::string_view log)
{
    print(fmt::color::light_gray, "INFO", log);
}

void Logger::debug(const std::string_view log)
{
    print(fmt::color::cornflower_blue, "DEBUG", log);
}

void Logger::print(const fmt::v9::color color, const std::string_view logType, const std::string_view& log)
{
    const auto date = std::chrono::system_clock::now();  
    const auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(date.time_since_epoch());

    fmt::print(fg(color), "[{:<5}][{:%Y-%m-%d %H:%M:}{:%S}] | {}\n", logType, date, milis, log);
}

}  // namespace engine
