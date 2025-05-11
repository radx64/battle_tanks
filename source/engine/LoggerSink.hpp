#pragma once

#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <string_view>

#include <fmt/color.h>

namespace engine
{
    
struct Log
{
    fmt::v11::color color;
    std::string logType;
    std::string prefix;
    std::string text;
    decltype(std::chrono::system_clock::now()) date;
    decltype(std::chrono::milliseconds()) milis;
};

class LoggerSink
{
public:
    static LoggerSink& instance();

    LoggerSink(const LoggerSink&) = delete;
    LoggerSink& operator=(const LoggerSink&) = delete;
    
    void log(const fmt::v11::color& color, 
        const std::string& logType,
        const std::string& prefix,
        const std::string& text);

 protected:
    LoggerSink();
    ~LoggerSink();
    void stop();
    void processLogs();
    void print(const fmt::v11::color color, 
        const std::string_view logType,
        const std::string_view prefix,
        const std::string_view log);

    std::atomic<bool> stop_;
    std::thread logThread_;
    std::queue<Log> logQueue_;
    std::mutex logQueueMutex_;

};

}  // namespace engine
