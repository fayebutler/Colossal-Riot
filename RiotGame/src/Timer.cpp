
#include "Timer.h"

Timer::Timer()
{
    timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
    m_currentTime = 0;
    timer->start(1000);
}

void Timer::TimerSlot()
{
    m_currentTime = m_currentTime + 1;
}

long long Timer::getCurrentTime()
{
    return m_currentTime;
}
