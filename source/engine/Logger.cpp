#include "engine/Logger.hpp"

#include "engine/LoggerSink.hpp"
#include "engine/Unused.hpp"

namespace engine
{

Logger::Logger(const std::string_view prefix)
: prefix_{prefix}
{
}

void Logger::error(const std::string_view log) const
{
    LoggerSink::instance().log(fmt::color::crimson, "error", prefix_, std::string(log));
}

void Logger::warning(const std::string_view log) const
{
    LoggerSink::instance().log(fmt::color::dark_orange, "warn", prefix_, std::string(log));
}

void Logger::info(const std::string_view log) const
{
    LoggerSink::instance().log(fmt::color::green, "info", prefix_, std::string(log));
}

void Logger::debug(const std::string_view log) const
{
#ifdef NDEBUG
    UNUSED(log);
    return;
#else
    LoggerSink::instance().log(fmt::color::cornflower_blue, "debug", prefix_, std::string(log));
#endif
}

}  // namespace engine
