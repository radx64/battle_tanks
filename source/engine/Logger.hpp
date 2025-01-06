#pragma once

#include <string_view>
#include <string>

namespace engine
{

class Logger
{
public:
    Logger(const std::string_view prefix);

    void error(const std::string_view log);
    void warning(const std::string_view log);
    void info(const std::string_view log);
    void debug(const std::string_view log);

protected:
    std::string prefix_;
};

}  // namespace engine 
