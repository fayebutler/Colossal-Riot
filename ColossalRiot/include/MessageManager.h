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

#define MessageMgr MessageManager::instance()

class MessageManager
{
public:
  static MessageManager* instance();

  void sendMessage(int _senderID, int _receiverID, int _message, double _delay, float _extraInfo);
  void sendDelayedMessages();



private:
  std::set<Message> m_queue;

  void send(BaseGameEntity *_receiver, const Message &_message);

  MessageManager();
};

#endif // MESSAGEMANAGER_H
