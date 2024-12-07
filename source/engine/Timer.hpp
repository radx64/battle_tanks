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
    double getDelay();
    void notify();

    void setTimerService(TimerService* service);

    ~Timer();

protected:
    TimerService* service_;
    Notification notification_;
    double delay_;
};

}  // namespace engine
