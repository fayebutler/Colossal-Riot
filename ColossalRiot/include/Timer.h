#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>


class Timer
{
private:
    static long long m_currentTime;
    static long long m_startTime;

    static double m_timeElapsed;
    static double m_previousTickTime;
    static double m_timeScale;

public:
    Timer();

    void resetTimer();
    double getCurrentTime();
    double timeElapsed();
    void pauseTimer();

};


#endif // TIMER_H
