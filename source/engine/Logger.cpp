#include "engine/Logger.hpp"

#include "engine/LoggerSink.hpp"

namespace engine
{

Logger::Logger(const std::string_view prefix)
: prefix_{prefix}
{
}

void Logger::error(const std::string_view log)
{
    LoggerSink::print(fmt::color::crimson, "ERROR", prefix_, log);
}

void Logger::warning(const std::string_view log)
{
    LoggerSink::print(fmt::color::dark_orange, "WARN", prefix_, log);
}

void Logger::info(const std::string_view log)
{
    LoggerSink::print(fmt::color::light_gray, "INFO", prefix_, log);
}

void Logger::debug(const std::string_view log)
{
    LoggerSink::print(fmt::color::cornflower_blue, "DEBUG", prefix_, log);
}

}  // namespace engine
