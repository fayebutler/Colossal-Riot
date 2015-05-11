#ifndef MESSAGE_H
#define MESSAGE_H

#include <math.h>
#include <iostream>

struct Message
{
  int m_sender;
  int m_receiver;
  int m_message;
  double m_dispatchTime;
  float m_extraInfo;

  Message():m_sender(-1),
            m_receiver(-1),
            m_message(-1),
            m_dispatchTime(-1) {}

  Message(int _sender, int _receiver, int _message, double _dispatchTime, float _extraInfo = NULL):
    m_sender(_sender), m_receiver(_receiver), m_message(_message), m_dispatchTime(_dispatchTime), m_extraInfo(_extraInfo) {}

};

const double minDiff = 0.25;

inline bool operator==(const Message& m1, const Message& m2)
{
  return (fabs(m1.m_dispatchTime - m2.m_dispatchTime) < minDiff)&&
      (m1.m_sender == m2.m_sender)&&
      (m1.m_receiver == m2.m_receiver)&&
      (m1.m_message == m2.m_message);
}

inline bool operator<(const Message& m1, const Message& m2)
{
  if (m1 == m2)
  {
    return false;
  }
  else
  {
    return (m1.m_dispatchTime < m2.m_dispatchTime);
  }
}

inline std::ostream& operator<<(std::ostream& os, const Message& m)
{
  os<<"time: "<<m.m_dispatchTime
    <<"\nsender: "<<m.m_sender
    <<"\nreceiver: "<<m.m_receiver
    <<"\nmessage: "<<m.m_message;

  return os;
}

//DEREFERENCE FUNCTION


#endif // MESSAGE_H
