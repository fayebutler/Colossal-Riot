#include "Timer.h"

Timer::Timer()
{
    m_startTime = SDL_GetTicks();
    //m_timeElapsed = 0.0;
   // m_lastTimeElapsed =0.0;
    //m_lastTimeInTimeElapsed= m_startTime;
}

double Timer::getCurrentTime()
{
    m_currentTime=SDL_GetTicks();
    return (m_currentTime - m_startTime);
}

void Timer::resetTimer()
{
    m_startTime = SDL_GetTicks();
}

double Timer::timeElapsed() //sort this out!!
{
    m_lastTimeElapsed = m_timeElapsed;
    m_currentTime=SDL_GetTicks();
    m_timeElapsed = (m_currentTime - m_lastTimeInTimeElapsed);
    m_lastTimeInTimeElapsed = m_currentTime;

    return m_timeElapsed;
}
