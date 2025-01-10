#pragma once

#include <vector>

#include "engine/Clock.hpp"
#include "engine/Logger.hpp"

namespace engine
{

class Timer;

enum class TimerType
{
    OneShot,
    Repeating
};

struct TimerInstance
{
    Timer* timer;
    TimerType type;
    Clock::time_point nextTick;
    bool active;
};

class TimerService final
{
public:
    TimerService();

    TimerService(const TimerService&) = delete;
    TimerService(TimerService&&) = delete;
    TimerService& operator=(const TimerService&) = delete;
    TimerService& operator=(TimerService&&) = delete;

    void update(const Clock::duration& delta);

    void start(Timer* timer, const TimerType type);
    void cancel(Timer* timer);
    const Clock::time_point& getCurrentTime() const;

protected:
    std::vector<TimerInstance> timers_;
    Clock::time_point currentTime_;
    Logger logger_;
};

}  // namespace engine
