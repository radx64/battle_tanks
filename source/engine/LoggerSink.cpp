#include "engine/LoggerSink.hpp"

#include <chrono>

#include <fmt/core.h>
#include <fmt/chrono.h>

namespace engine
{

LoggerSink& LoggerSink::instance()
{
    static LoggerSink instance;
    return instance;
}

void LoggerSink::processLogs()
{
    while (!stop_) 
    {
        {
            std::unique_lock<std::mutex> lock(logQueueMutex_);
            while (!logQueue_.empty()) 
            {
                const auto logEvent = logQueue_.front();
                logQueue_.pop();
                lock.unlock();

                print(logEvent.color, logEvent.logType, logEvent.prefix, logEvent.text);

                // Reacquire the lock and check `stop_` again
                lock.lock();
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

LoggerSink::LoggerSink()
: stop_{false}
, logThread_{&LoggerSink::processLogs, this}
{
}

LoggerSink::~LoggerSink()
{
    stop();
}

void LoggerSink::log(const fmt::v12::color& color, 
    const std::string& logType,
    const std::string& prefix,
    const std::string& text)
{
    auto date = std::chrono::system_clock::now();

    Log logEvent{
        .color = color,
        .logType = logType,
        .prefix = prefix,
        .text = text,
        .date = date,
        .milis = std::chrono::duration_cast<std::chrono::milliseconds>(date.time_since_epoch())
    };

    {
        std::lock_guard<std::mutex> lock(logQueueMutex_);
        logQueue_.push(logEvent);
    }

}

void LoggerSink::stop()
{
    stop_ = true;
    if (logThread_.joinable()) {
        logThread_.join();  // Wait for the thread to finish
    }
}

void LoggerSink::print(const fmt::v12::color color, 
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
