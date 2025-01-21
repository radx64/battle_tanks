#include "engine/Logger.hpp"

#include "engine/LoggerSink.hpp"

namespace engine
{

Logger::Logger(const std::string_view prefix)
: prefix_{prefix}
{
}

void Logger::error(const std::string_view log) const
{
    LoggerSink::print(fmt::color::crimson, "error", prefix_, log);
}

void Logger::warning(const std::string_view log) const
{
    LoggerSink::print(fmt::color::dark_orange, "warn", prefix_, log);
}

void Logger::info(const std::string_view log) const
{
    LoggerSink::print(fmt::color::green, "info", prefix_, log);
}

void Logger::debug(const std::string_view log) const
{
    LoggerSink::print(fmt::color::cornflower_blue, "debug", prefix_, log);
}

}  // namespace engine
