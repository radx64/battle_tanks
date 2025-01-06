#include "engine/LoggerSink.hpp"

#include <chrono>

namespace engine
{

void LoggerSink::print(const fmt::v9::color color, 
    const std::string_view logType, 
    const std::string_view prefix,
    const std::string_view log)
{
    const auto date = std::chrono::system_clock::now();  
    const auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(date.time_since_epoch());

    fmt::print("[{:%Y-%m-%d %H:%M:}{:%S}] [", date, milis);
    fmt::print(fg(color), "{:<5}", logType);
    fmt::print("] {}: {}\n", prefix, log);
}

}  // namespace engine
