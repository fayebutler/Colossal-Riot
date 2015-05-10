#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <set>
#include "Message.h"
#include "EntityManager.h"

class BaseGameEntity;
class EntityManager;

//----------------------------------------------------------------------------------------------------------------------
/// @file MessageManager.h
/// @class MessageManager "include/MessageManager.h"
/// @brief A manager monostate class that helps handle the sending and receiving of messages between entities
/// @author Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// @brief enum to describe the type of message that was sent
//----------------------------------------------------------------------------------------------------------------------

enum messageType
{
  msgAttack,      // 0
  msgRioterDeath, // 1
  msgPoliceDeath  // 2
};

class MessageManager
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor contrsucts the message manager and gives it access to the entity manager
  //----------------------------------------------------------------------------------------------------------------------
  MessageManager();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to send a message from a sender to a reciever with any additional information
  /// @param[in] _senderID the ID of the entity that wants to send a message
  /// @param[in] _recieverID the ID of the entity that will receive the message
  /// @param[in] _message the message type that will be sent
  /// @param[in] _extraInfo information that is used by the reciever in a way specified by the message type
  //----------------------------------------------------------------------------------------------------------------------
  void sendMessage(int _senderID, int _receiverID, int _message, float _extraInfo);

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the entity manager
  //----------------------------------------------------------------------------------------------------------------------
  static EntityManager* s_entityMgr;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method that ensures that messages are handled successfully
  //----------------------------------------------------------------------------------------------------------------------
  void send(BaseGameEntity *_receiver, const Message &_message);
};

#endif // MESSAGEMANAGER_H
