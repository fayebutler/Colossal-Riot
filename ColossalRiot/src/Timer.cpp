//----------------------------------------------------------------------------------------------------------------------
/// @file Timer.cpp
/// @brief A monostate class that contains all the time information to be used throughout the game
//----------------------------------------------------------------------------------------------------------------------

#include "Timer.h"

long long Timer::s_currentTime;
long long Timer::s_startTime;
double    Timer::s_timeElapsed;
double    Timer::s_previousTickTime;

//----------------------------------------------------------------------------------------------------------------------
Timer::Timer()
{
  s_startTime = SDL_GetTicks();
  s_timeElapsed = 0.0;
  s_previousTickTime= s_startTime;
}

//----------------------------------------------------------------------------------------------------------------------
double Timer::getCurrentTime()
{
  s_currentTime=SDL_GetTicks();
  return (s_currentTime - s_startTime)/1000.f;
}

//----------------------------------------------------------------------------------------------------------------------
void Timer::resetTimer()
{
  s_previousTickTime = 0;
  s_startTime = SDL_GetTicks();
}

//----------------------------------------------------------------------------------------------------------------------
void Timer::calculateTimeElapsed()
{
  s_currentTime = SDL_GetTicks() - s_startTime;
  s_timeElapsed = (s_currentTime - s_previousTickTime) / 1000.f;
  s_previousTickTime = s_currentTime;
}
