#include "TimerService.hpp"

#include <algorithm>

#include <fmt/format.h>

#include "Timer.hpp"


namespace engine
{

// FIXME:
// Timer service is not doing cleanups properlyu
// so over time it leaks with old timers that were destroyed
// funny things may happen if new timer will land in same memory address

TimerService::TimerService()
: currentTime_{}
, logger_{"TimerService"}
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
        logger_.debug(fmt::format("Firing timer {}", fmt::ptr(timerInstance.timer)));
        timerInstance.timer->notify();
    }

    timers_.erase(std::remove_if(timers_.begin(), timers_.end(),
        [](const auto timerInstance){ return not timerInstance.active; }),
        timers_.end());

}

void TimerService::start(Timer* timer, const TimerType type)
{
    if (std::find_if(std::begin(timers_), std::end(timers_),
        [&timer](const auto timerInstance){ return timerInstance.timer == timer; }) != timers_.end())
    {
        logger_.error(fmt::format("Timer {} already started", fmt::ptr(timer)));
        return;
    }

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
    auto timerInstanceIt = std::find_if(std::begin(timers_), std::end(timers_),
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
