#pragma once

#include <vector>

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
    double nextTick;
    bool active;

};

class TimerService final
{
public:
    TimerService();
    void update(double dt);

    void start(Timer* timer, const TimerType type);
    void cancel(Timer* timer);

protected:
    std::vector<TimerInstance> timers_;
    double currentTime_;
};

}  // namespace engine
