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
    while (!stop_ || !log_queue_.empty())
    {
        {
            std::unique_lock<std::mutex> lock(log_queue_mutex_);
            while (!log_queue_.empty()) 
            {
                const auto logEvent = log_queue_.front();
                log_queue_.pop();
                lock.unlock();

                print(logEvent.color, logEvent.log_type, logEvent.prefix, logEvent.text);

                // Reacquire the lock and check `stop_` again
                lock.lock();
            }
        }
        if (!stop_)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

LoggerSink::LoggerSink()
: stop_{false}
, log_thread_{&LoggerSink::processLogs, this}
{
}

LoggerSink::~LoggerSink()
{
    stop();
}

void LoggerSink::log(const fmt::v12::color& color, 
    const std::string& log_type,
    const std::string& prefix,
    const std::string& text)
{
    // Don't accept new logs during shutdown
    if (stop_)
    {
        return;
    }

    auto date = std::chrono::system_clock::now();

    Log logEvent{
        .color = color,
        .log_type = log_type,
        .prefix = prefix,
        .text = text,
        .date = date,
        .milis = std::chrono::duration_cast<std::chrono::milliseconds>(date.time_since_epoch())
    };

    {
        std::lock_guard<std::mutex> lock(log_queue_mutex_);
        log_queue_.push(logEvent);
    }

}

void LoggerSink::stop()
{
    stop_ = true;
    if (log_thread_.joinable()) {
        log_thread_.join();  // Wait for the thread to finish
    }
}

void LoggerSink::print(const fmt::v12::color color, 
    const std::string_view log_type, 
    const std::string_view prefix,
    const std::string_view log)
{
    const auto date = std::chrono::system_clock::now();  
    const auto milis = std::chrono::duration_cast<std::chrono::milliseconds>(date.time_since_epoch());

    const auto formatted = fmt::format("[{:%Y-%m-%d %H:%M:}{:%S}] [{:<5}] {}: {}\n",
        date,
        milis,
        log_type,
        prefix,
        log);

    fmt::print(fg(color), "{}", formatted);
}

}  // namespace engine
