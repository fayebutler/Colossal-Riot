#ifndef MESSAGE_H
#define MESSAGE_H

#include <math.h>
#include <iostream>

struct Message
{
  int m_sender;
  int m_receiver;
  int m_message;
  float m_extraInfo;

  Message():m_sender(-1),
            m_receiver(-1),
            m_message(-1) {}

  Message(int _sender, int _receiver, int _message,  float _extraInfo = NULL):
    m_sender(_sender), m_receiver(_receiver), m_message(_message), m_extraInfo(_extraInfo) {}

};

inline std::ostream& operator<<(std::ostream& os, const Message& m)
{
  os<<"\nsender: "<<m.m_sender
    <<"\nreceiver: "<<m.m_receiver
    <<"\nmessage: "<<m.m_message;

  return os;
}

#endif // MESSAGE_H
