//----------------------------------------------------------------------------------------------------------------------
/// @file MessageManager.cpp
/// @brief A manager monostate class that helps handle the sending and receiving of messages between entities
//----------------------------------------------------------------------------------------------------------------------

#include "MessageManager.h"

//----------------------------------------------------------------------------------------------------------------------
MessageManager::MessageManager()
{
}
MessageManager::~MessageManager()
{
}

//----------------------------------------------------------------------------------------------------------------------
void MessageManager::sendMessage(int _senderID, int _receiverID, int _message, float _extraInfo)
{
  EntityManager* entityMgr;
  entityMgr = new EntityManager();
  BaseGameEntity* receiver = (BaseGameEntity*)entityMgr->getEntityFromID(_receiverID);
  if (receiver == NULL)
  {
    std::cout<<"Error: No receiver with ID of "<<_receiverID<<" found"<<std::endl;
    return;
  }
  Message message(_senderID, _receiverID, _message, _extraInfo);
  send(receiver, message);
  delete entityMgr;
}

//----------------------------------------------------------------------------------------------------------------------
void MessageManager::send(BaseGameEntity *_receiver, const Message &_message)
{
  if (_receiver->handleMessage(_message) == false)
  {
    std::cout<<"MessageManager::discharge: message could not be handled"<<std::endl;
  }
}


