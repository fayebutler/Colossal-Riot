#ifndef MESSAGE_H
#define MESSAGE_H

//----------------------------------------------------------------------------------------------------------------------
/// @file Message.h
/// @class Message "include/Message.h"
/// @brief A struct used to store message information so it can be sent from one entity to another
/// @author Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

#include <math.h>
#include <iostream>

struct Message
{
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor this will create an invalid message if it has not been used properly
  //----------------------------------------------------------------------------------------------------------------------
  Message():m_sender(-1), m_receiver(-1), m_message(-1) {}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor will create a message with required inputs
  //----------------------------------------------------------------------------------------------------------------------
  Message(int _sender, int _receiver, int _message,  float _extraInfo = NULL):
          m_sender(_sender), m_receiver(_receiver), m_message(_message), m_extraInfo(_extraInfo) {}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the entity that sends the message
  //----------------------------------------------------------------------------------------------------------------------
  int m_sender;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the entity that will receive the message
  //----------------------------------------------------------------------------------------------------------------------
  int m_receiver;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the message type that will be sent
  //----------------------------------------------------------------------------------------------------------------------
  int m_message;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief any extra information that will be used by the receiver as sepcified by the message
  //----------------------------------------------------------------------------------------------------------------------
  float m_extraInfo;
};

//----------------------------------------------------------------------------------------------------------------------
/// @brief overloading the ostream to allow for easier message debugging
/// @param[in] _os the std::ostream to overload
/// @param[in] _m the message that will have it's members printed
//----------------------------------------------------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& _os, const Message& _m)
{
  _os<<"\nsender: "  <<_m.m_sender
     <<"\nreceiver: "<<_m.m_receiver
     <<"\nmessage: " <<_m.m_message;

  return _os;
}

#endif // MESSAGE_H
