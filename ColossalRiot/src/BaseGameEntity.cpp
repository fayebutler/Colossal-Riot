#include "include/BaseGameEntity.h"

int BaseGameEntity::m_nextValidID = 0;

BaseGameEntity::BaseGameEntity()
{
  setID(m_nextValidID);
  m_nextValidID++;

  m_entityMgr = new EntityManager();
  m_entityMgr->registerEntity(this);
}

BaseGameEntity::BaseGameEntity(entityType entity_type, ngl::Vec3 pos, float r)
{
  setID(m_nextValidID);
  m_nextValidID++;

  m_entityMgr = new EntityManager();
  m_entityMgr->registerEntity(this);

  m_pos = pos;
  m_boundingRadius = r;
  m_entityType = entity_type;
  m_detectionRadius = m_boundingRadius*3; // CHANGE TO VARIABLE

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

float BaseGameEntity::getDetectionRadius()
{
    return m_detectionRadius;
}

void BaseGameEntity::setDetectionRadius(float _r)
{
  m_detectionRadius = _r;
}

std::vector<int> BaseGameEntity::getDetectedEntityIDs()
{
    return m_detectedDynamicEntityIDs;
}

void BaseGameEntity::addPoliceID(int _ID)
{
    m_neighbourPoliceIDs.push_back(_ID);
}

void BaseGameEntity::addRioterID(int _ID)
{
    m_neighbourRioterIDs.push_back(_ID);
}

void BaseGameEntity::clearAgentIDs()
{
    m_neighbourPoliceIDs.clear();
    m_neighbourRioterIDs.clear();
}

