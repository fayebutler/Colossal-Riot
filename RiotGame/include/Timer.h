#ifndef TIMER_H
#define TIMER_H
#include <iostream>

#include "QTimer"

class Timer : public QObject
{
    Q_OBJECT

private:

    long long m_currentTime;

    double m_timeElapsed;

public:
    Timer();
    QTimer *timer;
    long long getCurrentTime();

public slots:
    void TimerSlot();
};

#endif // TIMER_H
