#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <set>
#include "Message.h"
#include "EntityManager.h"



enum messageType
{
  msgAttack
};

class BaseGameEntity;
class EntityManager;

//#define MessageMgr MessageManager::instance()

class MessageManager
{
public:

  MessageManager();
  //static MessageManager* instance();

  void sendMessage(int _senderID, int _receiverID, int _message, double _delay, float _extraInfo);
  void sendDelayedMessages();



private:

  static std::set<Message> m_queue;

  void send(BaseGameEntity *_receiver, const Message &_message);


};

#endif // MESSAGEMANAGER_H
