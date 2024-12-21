#include "Timer.hpp"

#include "TimerService.hpp"

namespace engine
{

Timer::Timer(const Clock::duration& delay, Timer::Notification notification)
: service_{nullptr}
, notification_(notification)
, delay_(delay)
{

}

Timer::~Timer()
{
    if (service_ != nullptr) service_->cancel(this);
}

void Timer::setTimerService(TimerService* service)
{
    service_ = service;
}

const Clock::duration& Timer::getDelay() const
{
    return delay_;
}

void Timer::setDelay(const Clock::duration& delay)
{
    delay_ = delay;
}

void Timer::notify()
{
    notification_();
}

}  // namespace engine
