#pragma once

#include <functional>

#include "engine/Clock.hpp"

namespace engine {class TimerService;}

namespace engine
{

class Timer
{
public:
    using Notification = std::function<void(void)>;

    Timer(const Clock::duration& delay, Notification notification);

    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&) = delete;

    const Clock::duration& getDelay() const;
    void setDelay(const Clock::duration& delay);
    void notify();

    ~Timer();

protected:
    friend class TimerService;

    void setTimerService(TimerService* service);
    TimerService* service_;
    Notification notification_;
    Clock::duration delay_;
};

}  // namespace engine
