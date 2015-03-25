#include "BaseEntity.h"

int BaseEntity::m_nextValidID = 0;

BaseEntity::BaseEntity()
{
    setID(m_nextValidID);
    m_nextValidID++;
    std::cout<<"ID = "<<m_ID<<std::endl;

    m_rage = 5.0;

    EntityMgr->registerEntity(this);
}

BaseEntity::~BaseEntity()
{

}

void BaseEntity::update()
{

}

bool BaseEntity::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {
  case msgMoraleUp:
    std::cout<<"Message morale up: morale = "<<m_morale<<std::endl;
    m_morale += _message.m_extraInfo;
    std::cout<<"Message morale up: morale = "<<m_morale<<std::endl;
    return true;

  case msgMoraleDown:
    std::cout<<"Message morale down: morale = "<<m_morale<<std::endl;
    m_morale -= _message.m_extraInfo;
    std::cout<<"Message morale down: morale = "<<m_morale<<std::endl;
    return true;

  default:
    std::cout<<"Message type not defined"<<std::endl;
    return false;
  }
}
