#include "TimerService.hpp"

#include <algorithm>

#include "Timer.hpp"

#include <iostream>

namespace engine
{

TimerService::TimerService()
: currentTime_{}
{

}
void TimerService::update(const Clock::duration& delta)
{
    currentTime_ += delta;

    for (auto& timerInstance : timers_)
    {
        if ((currentTime_ < timerInstance.nextTick) || not timerInstance.active)
        {
            continue;
        }

        // TODO: consider more precise approach
        // where delta between current tick and next tick
        // is taken into use to not aggregate deviation
        // when update method is called in uneven stepts
        timerInstance.nextTick += timerInstance.timer->getDelay();

        if (timerInstance.type == TimerType::OneShot)
        {
            timerInstance.active = false;
        }
        timerInstance.timer->notify();
    }
}

void TimerService::start(Timer* timer, const TimerType type)
{
    timers_.emplace_back(
        TimerInstance
        {
            .timer = timer,
            .type = type,
            .nextTick = currentTime_ + timer->getDelay(),
            .active = true
        }
    );

    timer->setTimerService(this);
}

void TimerService::cancel(Timer* timer)
{
    auto timerInstanceIt = std::find_if(timers_.begin(), timers_.end(),
        [&timer](const auto timerInstance){ return timerInstance.timer == timer; });

    if (timerInstanceIt != timers_.end())
    {
        timerInstanceIt->active = false;
    }
}

const Clock::time_point& TimerService::getCurrentTime() const
{
    return currentTime_;
} 

}  // namespace engine
