#include "Timer.h"
#include <iostream>

long long Timer::m_currentTime;
long long Timer::m_startTime;
double    Timer::m_timeElapsed;
double    Timer::m_previousTickTime;
double    Timer::m_timeScale;

Timer::Timer()
{
    m_startTime = SDL_GetTicks();
    m_timeElapsed = 0.0;
    m_previousTickTime= m_startTime;
}

double Timer::getCurrentTime()
{
    m_currentTime=SDL_GetTicks();
    return (m_currentTime - m_startTime)/1000.0;
}

void Timer::resetTimer()
{
    m_previousTickTime = 0;
    m_startTime = SDL_GetTicks();
}

double Timer::timeElapsed()
{

    m_currentTime = SDL_GetTicks() - m_startTime;
    m_timeElapsed = (m_currentTime - m_previousTickTime);
    m_previousTickTime = m_currentTime;

    return m_timeElapsed/1000.0;
}
