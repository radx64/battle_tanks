#include "engine/LoggerSink.hpp"

#include <chrono>

namespace engine
{

LoggerSink& LoggerSink::instance()
{
    static LoggerSink instance;
    return instance;
}

void LoggerSink::processLogs()
{
    while (running_ || !logQueue.empty()) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, [this] { return !logQueue.empty() || !running_; });

        while (!logQueue.empty()) {

            const auto logEvent = logQueue.front();
            logQueue.pop();
            lock.unlock();

            print(logEvent.color, logEvent.logType, logEvent.prefix, logEvent.text);

            lock.lock();
        }
    }
};

LoggerSink::LoggerSink(): running_{true}, logThread{&LoggerSink::processLogs, this}
{
}

void LoggerSink::log(const fmt::v9::color color, 
    const std::string logType,
    const std::string prefix,
    const std::string text)
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
        std::lock_guard<std::mutex> lock(queueMutex);
        logQueue.push(logEvent);
    }
    cv.notify_one(); 
}

void LoggerSink::stop()
{
    running_ = false;
    cv.notify_one();
    if (logThread.joinable()) {
        logThread.join();
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
