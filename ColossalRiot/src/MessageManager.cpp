#include "MessageManager.h"

EntityManager* MessageManager::m_entityMgr;

MessageManager::MessageManager()
{
  m_entityMgr = new EntityManager();
}
MessageManager::~MessageManager()
{
    //delete m_entityMgr;
}

void MessageManager::sendMessage(int _senderID, int _receiverID, int _message, float _extraInfo)
{
  //EntityManager* entityMgr;
  //entityMgr = new EntityManager();
  BaseGameEntity* receiver = (BaseGameEntity*)m_entityMgr->getEntityFromID(_receiverID);


  if (receiver == NULL)
  {
    std::cout<<"Error: No receiver with ID of "<<_receiverID<<" found"<<std::endl;
    return;
  }

  Message message(_senderID, _receiverID, _message, _extraInfo);

  send(receiver, message);
  //delete entityMgr;
}

void MessageManager::send(BaseGameEntity *_receiver, const Message &_message)
{
  if (_receiver->handleMessage(_message) == false)
  {
    std::cout<<"MessageManager::discharge: message could not be handled"<<std::endl;
  }
}


