#pragma once

#include <functional>

namespace engine {class TimerService;}

namespace engine
{

class Timer
{
public:
    using Notification = std::function<void(void)>;

    Timer(const double delay, Notification notification);

    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;

    double getDelay();
    void notify();

    ~Timer();

protected:
    friend class TimerService;

    void setTimerService(TimerService* service);
    TimerService* service_;
    Notification notification_;
    double delay_;
};

}  // namespace engine
