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
    while (running_ || !logQueue_.empty()) {
        std::unique_lock<std::mutex> lock(logQueueMutex_);
        cv_.wait(lock, [this]
        { 
            return !logQueue_.empty() || !running_; 
        });

        while (!logQueue_.empty()) 
        {
            const auto logEvent = logQueue_.front();
            logQueue_.pop();
            lock.unlock();

            print(logEvent.color, logEvent.logType, logEvent.prefix, logEvent.text);

            lock.lock();
        }
    }
};

LoggerSink::LoggerSink()
: running_{true}
, logThread_{&LoggerSink::processLogs, this}
{
}

LoggerSink::~LoggerSink()
{
    if (running_)
    {
        stop();
    }
}

void LoggerSink::log(const fmt::v9::color& color, 
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
    cv_.notify_one(); 
}

void LoggerSink::stop()
{
    running_ = false;
    cv_.notify_one();
    if (logThread_.joinable()) 
    {
        logThread_.join();
    }
}

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
