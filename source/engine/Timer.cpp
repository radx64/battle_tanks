#include "Timer.hpp"

#include "TimerService.hpp"

namespace engine
{

Timer::Timer(const double delay, Timer::Notification notification)
: notification_(notification)
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

double Timer::getDelay()
{
    return delay_;
}

void Timer::notify()
{
    notification_();
}

}  // namespace engine
