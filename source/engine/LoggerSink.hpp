#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string_view>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/chrono.h>

namespace engine
{
    
struct Log
{
    fmt::v9::color color;
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

    void stop();
    
    LoggerSink(const LoggerSink&) = delete;
    LoggerSink& operator=(const LoggerSink&) = delete;
    
protected:
    LoggerSink();
    ~LoggerSink() = default;

    void log(const fmt::v9::color color, 
        const std::string logType,
        const std::string prefix,
        const std::string text);


    void processLogs();

    static void print(const fmt::v9::color color, 
        const std::string_view logType,
        const std::string_view prefix,
        const std::string_view log);

    friend class Logger;

    std::atomic<bool> running_;
    std::thread logThread;
    std::queue<Log> logQueue;
    std::mutex queueMutex;
    std::condition_variable cv;

};

}  // namespace engine
