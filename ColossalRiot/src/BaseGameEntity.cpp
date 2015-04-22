#include "include/BaseGameEntity.h"

int BaseGameEntity::m_nextValidID = 0;

BaseGameEntity::BaseGameEntity()
{
  setID(m_nextValidID);
  m_nextValidID++;

  EntityMgr->registerEntity(this);
}

BaseGameEntity::BaseGameEntity(entityType entity_type, ngl::Vec3 pos, float r)
{
   setID(m_nextValidID);
   m_nextValidID++;

   EntityMgr->registerEntity(this);

    m_pos = pos;
    m_boundingRadius = r;
    m_entityType = entity_type;

}

void BaseGameEntity::setCurrentCellID(int _ID)
{
    m_currentCellID = _ID;
}

int BaseGameEntity::getCurrentCell()
{
    return m_currentCellID;
}

bool BaseGameEntity::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {

  default:
    std::cout<<"BaseGameEntity: Message type not defined"<<std::endl;
    return false;
  }
}

void BaseGameEntity::addDetectedDynamicEntityID(int _ID)
{
    m_detectedDynamicEntityIDs.push_back(_ID);
}

void BaseGameEntity::clearDetectedDynamicEntityID()
{
    m_detectedDynamicEntityIDs.clear();
}
