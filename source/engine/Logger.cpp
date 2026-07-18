#include "engine/Logger.hpp"

#include "engine/LoggerSink.hpp"
#include "engine/Unused.hpp"

namespace engine
{

Logger::Logger(const std::string_view prefix)
: prefix_{prefix}
{
}

void Logger::vip(const std::string_view log) const
{
    LoggerSink::instance().log(fmt::color::rebecca_purple, "vip", prefix_, std::string(log));
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

void Logger::debug(const std::string_view log, std::source_location location) const
{
#ifdef NDEBUG
    UNUSED(log);
    UNUSED(location);
    return;
#else
    std::string file = location.file_name();
    size_t last_slash_pos = file.find_last_of("/\\");
    if (last_slash_pos != std::string::npos)    {
        file = file.substr(last_slash_pos + 1);
    }

    LoggerSink::instance().log(fmt::color::cornflower_blue, "debug", "(" + file + ":" + std::to_string(location.line()) + ") " + prefix_, std::string(log));
#endif
}

}  // namespace engine
