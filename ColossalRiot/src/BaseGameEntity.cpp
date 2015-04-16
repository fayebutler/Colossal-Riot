#include "include/BaseGameEntity.h"

int BaseGameEntity::m_nextValidID = 0;

BaseGameEntity::BaseGameEntity()
{
  setID(m_nextValidID);
  m_nextValidID++;

  EntityMgr->registerEntity(this);
}

BaseGameEntity::BaseGameEntity(int entity_type, ngl::Vec3 pos, float r)
{
   setID(m_nextValidID);
   m_nextValidID++;

   EntityMgr->registerEntity(this);

    m_pos = pos;
    m_boundingRadius = r;
    m_entityType = entity_type;

}

bool BaseGameEntity::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {
  case msgAttack:
    //m_health -= _message.m_extraInfo;
    return true;

  default:
    std::cout<<"Message type not defined"<<std::endl;
    return false;
  }
}
