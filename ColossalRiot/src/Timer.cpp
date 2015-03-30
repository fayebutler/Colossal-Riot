#include "Timer.h"

Timer::Timer()
{
    m_startTime = SDL_GetTicks();
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
