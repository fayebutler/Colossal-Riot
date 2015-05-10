#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <set>
#include "Message.h"
#include "EntityManager.h"



enum messageType
{
  msgAttack,
  msgRioterDeath,
  msgPoliceDeath
};

class BaseGameEntity;
class EntityManager;

class MessageManager
{
public:

  MessageManager();
  ~MessageManager();

  void sendMessage(int _senderID, int _receiverID, int _message, float _extraInfo);

private:

  static std::set<Message> m_queue;
  static EntityManager* m_entityMgr;

  void send(BaseGameEntity *_receiver, const Message &_message);


};

#endif // MESSAGEMANAGER_H
