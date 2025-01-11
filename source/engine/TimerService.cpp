#include "TimerService.hpp"

#include <algorithm>

#include <fmt/format.h>

#include "Timer.hpp"


namespace engine
{

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
        // is taken into account to not aggregate deviation
        // when update method is called in uneven steps
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

    startImpl(timer, type);

    timer->setTimerService(this);
}

void TimerService::startImpl(Timer* timer, const TimerType type)
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
}

void TimerService::restart(Timer* timer, const TimerType type)
{
    auto timerInstanceIt = std::find_if(std::begin(timers_), std::end(timers_),
        [&timer](const auto timerInstance){ return timerInstance.timer == timer; });

    if (timerInstanceIt != timers_.end())
    {
        timerInstanceIt->nextTick = currentTime_ + timer->getDelay();
        timerInstanceIt->active = true;
        timerInstanceIt->type = type;
    }
    else
    {
        startImpl(timer, type);
    }
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
