#pragma once

#include <string_view>
#include <string>

namespace engine
{

class Logger
{
public:
    Logger(const std::string_view prefix);

    void vip(const std::string_view log) const;
    void error(const std::string_view log) const;
    void warning(const std::string_view log) const;
    void info(const std::string_view log) const;
    void debug(const std::string_view log) const;

protected:
    std::string prefix_;
};

}  // namespace engine 
