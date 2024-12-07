#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "engine/Timer.hpp"
#include "engine/TimerService.hpp"

namespace engine
{

class TimerClientStub
{
public:
    MOCK_METHOD(void, notificaiton, ());
};

TEST(TimerServiceShould, callTimerWhenIsOver)
{
    TimerClientStub timerClientStub;

    TimerService timerService{};
    Timer timer(0.5, [&timerClientStub]()
    {
        timerClientStub.notificaiton();
    });

    timerService.start(&timer, TimerType::OneShot);
    EXPECT_CALL(timerClientStub, notificaiton).Times(1);
    timerService.update(0.6);

}

TEST(TimerServiceShould, CallRepeatingTimerEveryInterval)
{
    TimerService timerService{};

    TimerClientStub timerClientStub;
    TimerClientStub timer2ClientStub;

    Timer timer(0.40, [&timerClientStub]()
    {
        timerClientStub.notificaiton();
    });

    Timer timer2(0.30, [&timer2ClientStub]()
    {
        timer2ClientStub.notificaiton();
    });

    timerService.start(&timer, TimerType::Repeating);
    timerService.start(&timer2, TimerType::Repeating);

    EXPECT_CALL(timerClientStub, notificaiton).Times(2);
    EXPECT_CALL(timer2ClientStub, notificaiton).Times(3);

    for (size_t i = 0; i < 10; ++i)
    {
        timerService.update(0.1);
    }
}

TEST(TimerServiceShould, CallOneShotTimerOnlyOnce)
{
    TimerService timerService{};

    TimerClientStub timerClientStub;
    TimerClientStub timer2ClientStub;

    Timer timer(0.40, [&timerClientStub]()
    {
        timerClientStub.notificaiton();
    });

    Timer timer2(0.30, [&timer2ClientStub]()
    {
        timer2ClientStub.notificaiton();
    });

    timerService.start(&timer, TimerType::OneShot);
    timerService.start(&timer2, TimerType::OneShot);

    EXPECT_CALL(timerClientStub, notificaiton).Times(1);
    EXPECT_CALL(timer2ClientStub, notificaiton).Times(1);

    for (size_t i = 0; i < 10; ++i)
    {
        timerService.update(0.1);
    }
}

TEST(TimerServiceShould, NotCallCanceledTimer)
{
    TimerService timerService{};

    TimerClientStub timerClientStub;
    TimerClientStub timer2ClientStub;

    Timer timer(0.40, [&timerClientStub]()
    {
        timerClientStub.notificaiton();
    });

    Timer timer2(0.30, [&timer2ClientStub]()
    {
        timer2ClientStub.notificaiton();
    });

    timerService.start(&timer, TimerType::OneShot);
    timerService.start(&timer2, TimerType::OneShot);

    EXPECT_CALL(timerClientStub, notificaiton).Times(1);
    EXPECT_CALL(timer2ClientStub, notificaiton).Times(0);

    timerService.cancel(&timer2);

    for (size_t i = 0; i < 10; ++i)
    {
        timerService.update(0.1);
    }
}

TEST(TimerServiceShould, HandleTimerGoingOutOfScope)
{
    TimerService timerService{};

    TimerClientStub timerClientStub;
    TimerClientStub timer2ClientStub;

    Timer timer(0.40, [&timerClientStub]()
    {
        timerClientStub.notificaiton();
    });

    timerService.start(&timer, TimerType::OneShot);

    {
        Timer timer2(0.30, [&timer2ClientStub]()
        {
            timer2ClientStub.notificaiton();
        });

        timerService.start(&timer2, TimerType::OneShot);
    }

    EXPECT_CALL(timerClientStub, notificaiton).Times(1);
    EXPECT_CALL(timer2ClientStub, notificaiton).Times(0);

    for (size_t i = 0; i < 10; ++i)
    {
        timerService.update(0.1);
    }
}


}  // namespace engine
