#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>


class Timer
{
private:
    long long m_currentTime;
    long long m_startTime;

    double m_timeElapsed;
    double m_previousTickTime;
    double m_timeScale;

public:
    Timer();

    void resetTimer();
    double getCurrentTime();
    double timeElapsed();
    void pauseTimer();

};


#endif // TIMER_H
