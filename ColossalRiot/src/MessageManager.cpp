#include "MessageManager.h"

MessageManager::MessageManager()
{
}

MessageManager* MessageManager::instance()
{
  static MessageManager instance;

  return &instance;
}

void MessageManager::sendMessage(int _senderID, int _receiverID, int _message, double _delay, float _extraInfo)
{
  BaseGameEntity* sender = (BaseGameEntity*) EntityMgr->getEntityFromID(_senderID);
  BaseGameEntity* receiver = (BaseGameEntity*)EntityMgr->getEntityFromID(_receiverID);

  if (receiver == NULL)
  {
    std::cout<<"Error: No receiver with ID of "<<_receiverID<<" found"<<std::endl;
    return;
  }

  Message message(_senderID, _receiverID, _message, 0, _extraInfo); //SHOULD THE 0 BE CURRENT TIME?

  if (_delay<=0.0f)
  {
    send(receiver, message);
  }
  else
  {
    // WE NEED CLOCK FUNCTIONALITY
  }
}

void MessageManager::sendDelayedMessages()
{
  // WE NEED CLOCK FUNCTIONALITY
}

void MessageManager::send(BaseGameEntity *_receiver, const Message &_message)
{
  if (_receiver->handleMessage(_message) == false)
  {
    std::cout<<"MessageManager::discharge: message could not be handled"<<std::endl;
  }
}
