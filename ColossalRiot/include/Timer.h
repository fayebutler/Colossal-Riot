#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>


class Timer
{
private:
    long long m_currentTime;
    long long m_startTime;

    double m_timeElapsed;

public:
    Timer();

    void resetTimer();
    double getCurrentTime();

   //bool readyForNextFrame();
};


#endif // TIMER_H
