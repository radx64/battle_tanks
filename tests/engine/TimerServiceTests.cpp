#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "engine/Timer.hpp"
#include "engine/TimerService.hpp"

using namespace std::literals;

namespace engine
{

class TimerClientStub
{
public:
    MOCK_METHOD(void, notification, ());
};

TEST(TimerServiceShould, returnCurrentTime)
{
    TimerService timerService{};

    EXPECT_EQ(timerService.getCurrentTime(), Clock::time_point(0ms));

    timerService.update(Clock::duration(100ms));
    EXPECT_EQ(timerService.getCurrentTime(), Clock::time_point(100ms));

    timerService.update(Clock::duration(300ms));
    EXPECT_EQ(timerService.getCurrentTime(), Clock::time_point(400ms));
}

TEST(TimerServiceShould, callTimerWhenIsOver)
{
    TimerClientStub timerClientStub{};

    TimerService timerService{};
    Timer timer{Clock::duration(500ms), [&timerClientStub]()
    {
        timerClientStub.notification();
    }};

    timerService.start(&timer, TimerType::OneShot);
    EXPECT_CALL(timerClientStub, notification).Times(1);
    timerService.update(Clock::duration(600ms));
}

TEST(TimerServiceShould, CallRepeatingTimerEveryInterval)
{
    TimerService timerService{};

    TimerClientStub timerClientStub{};
    TimerClientStub timer2ClientStub;

    Timer timer{Clock::duration(400ms), [&timerClientStub]()
    {
        timerClientStub.notification();
    }};

    Timer timer2(Clock::duration(300ms), [&timer2ClientStub]()
    {
        timer2ClientStub.notification();
    });

    timerService.start(&timer, TimerType::Repeating);
    timerService.start(&timer2, TimerType::Repeating);

    EXPECT_CALL(timerClientStub, notification).Times(2);
    EXPECT_CALL(timer2ClientStub, notification).Times(3);

    for (size_t i = 0; i < 10; ++i)
    {
        timerService.update(Clock::duration(100ms));
    }
}

TEST(TimerServiceShould, CallOneShotTimerOnlyOnce)
{
    TimerService timerService{};

    TimerClientStub timerClientStub{};
    TimerClientStub timer2ClientStub;

    Timer timer{Clock::duration(400ms), [&timerClientStub]()
    {
        timerClientStub.notification();
    }};

    Timer timer2(Clock::duration(300ms), [&timer2ClientStub]()
    {
        timer2ClientStub.notification();
    });

    timerService.start(&timer, TimerType::OneShot);
    timerService.start(&timer2, TimerType::OneShot);

    EXPECT_CALL(timerClientStub, notification).Times(1);
    EXPECT_CALL(timer2ClientStub, notification).Times(1);

    for (size_t i = 0; i < 10; ++i)
    {
        timerService.update(Clock::duration(100ms));
    }
}

TEST(TimerServiceShould, NotCallCanceledTimer)
{
    TimerService timerService{};

    TimerClientStub timerClientStub{};
    TimerClientStub timer2ClientStub;

    Timer timer{Clock::duration(400ms), [&timerClientStub]()
    {
        timerClientStub.notification();
    }};

    Timer timer2(Clock::duration(300ms), [&timer2ClientStub]()
    {
        timer2ClientStub.notification();
    });

    timerService.start(&timer, TimerType::OneShot);
    timerService.start(&timer2, TimerType::OneShot);

    EXPECT_CALL(timerClientStub, notification).Times(1);
    EXPECT_CALL(timer2ClientStub, notification).Times(0);

    timerService.cancel(&timer2);

    for (size_t i = 0; i < 10; ++i)
    {
        timerService.update(Clock::duration(100ms));
    }
}

TEST(TimerServiceShould, HandleTimerGoingOutOfScope)
{
    TimerService timerService{};

    TimerClientStub timerClientStub{};
    TimerClientStub timer2ClientStub;

    Timer timer{Clock::duration(400ms), [&timerClientStub]()
    {
        timerClientStub.notification();
    }};

    timerService.start(&timer, TimerType::OneShot);

    {
        Timer timer2(Clock::duration(300ms), [&timer2ClientStub]()
        {
            timer2ClientStub.notification();
        });

        timerService.start(&timer2, TimerType::OneShot);
    }

    EXPECT_CALL(timerClientStub, notification).Times(1);
    EXPECT_CALL(timer2ClientStub, notification).Times(0);

    for (size_t i = 0; i < 10; ++i)
    {
        timerService.update(Clock::duration(100ms));
    }
}

TEST(TimerServiceShould, RestartNotActiveTimerWithUpdateValues)
{
    TimerClientStub timerClientStub{};

    TimerService timerService{};
    Timer timer{Clock::duration(500ms), [&timerClientStub]()
    {
        timerClientStub.notification();
    }};

    timerService.start(&timer, TimerType::OneShot);
    EXPECT_CALL(timerClientStub, notification).Times(1);
    timerService.update(Clock::duration(600ms));

    timer.setDelay(Clock::duration(2000ms));

    timerService.start(&timer, TimerType::OneShot);

    EXPECT_CALL(timerClientStub, notification).Times(0);
    timerService.update(Clock::duration(1000ms));

    EXPECT_CALL(timerClientStub, notification).Times(1);
    timerService.update(Clock::duration(1000ms));
}

TEST(TimerServiceShould, ProperlyCancelNewTimerWithPreviouslyUsedAddress)
{
    TimerClientStub timerClientStub{};

    TimerService timerService{};
    Timer timer{Clock::duration(60s), [&timerClientStub]()
    {
        timerClientStub.notification();
    }};

    timerService.start(&timer, TimerType::OneShot);
    timerService.cancel(&timer);
    timerService.start(&timer, TimerType::OneShot);
    timerService.cancel(&timer);
    
    EXPECT_CALL(timerClientStub, notification).Times(0);
    timerService.update(Clock::duration(61s));
}

TEST(TimerServiceShould, RestartCancelledButNotYetRemovedTimer)
{
    TimerClientStub timerClientStub{};

    TimerService timerService{};

    Timer timer{Clock::duration(60s), [&timerClientStub]()
    {
        timerClientStub.notification();
    }};

    timerService.start(&timer, TimerType::OneShot);
    timerService.cancel(&timer);
    timerService.restart(&timer, TimerType::OneShot);

    EXPECT_CALL(timerClientStub, notification).Times(1);
    timerService.update(Clock::duration(61s));
}

TEST(TimerServiceShould, StartTimerUsingRestartMethod)
{
    TimerClientStub timerClientStub{};

    TimerService timerService{};

    Timer timer{Clock::duration(60s), [&timerClientStub]()
    {
        timerClientStub.notification();
    }};

    timerService.restart(&timer, TimerType::OneShot);
    EXPECT_CALL(timerClientStub, notification).Times(1);
    timerService.update(Clock::duration(61s));
}

}  // namespace engine
