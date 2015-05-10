#include "BaseGameEntity.h"
#include "GameWorld.h"

int BaseGameEntity::m_nextValidID = 0;

BaseGameEntity::BaseGameEntity()
{
  setID(m_nextValidID);
  m_nextValidID++;

  m_entityMgr = new EntityManager();
  m_entityMgr->registerEntity(this);
}


BaseGameEntity::BaseGameEntity(GameWorld* world, entityType entity_type, ngl::Vec3 pos, float r)
{
  m_world = world;

  if(m_world->getResetID()==1)
  {
      m_nextValidID = 0;
      m_world->setResetID(0);
  }

  //m_nextValidID = m_entityMgr->getSize();

  setID(m_nextValidID);
  m_nextValidID++;

  m_messageMgr = new MessageManager();

  m_entityMgr = new EntityManager();
  m_entityMgr->registerEntity(this);

  m_pos = pos;
  m_boundingRadius = r;
  m_entityType = entity_type;
  m_detectionRadius = r * 3; // CHANGE TO VARIABLE

  m_world->getCellGraph()->initializeCells(this);

}

BaseGameEntity::~BaseGameEntity()
{
  delete m_entityMgr;
  delete m_messageMgr;
}


void BaseGameEntity::setCurrentCellID(int _ID)
{
    m_currentCellID = _ID;
}

int BaseGameEntity::getCurrentCellID()
{
    return m_currentCellID;
}

void BaseGameEntity::setCurrentCell(Cell _inCell)
{
  m_currentCell = _inCell;
}

Cell BaseGameEntity::getCurrentCell()
{
  return m_currentCell;
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

void BaseGameEntity::addObstacleID(int _ID)
{
    m_neighbourObstacleIDs.push_back(_ID);
}

void BaseGameEntity::addRioterID(int _ID)
{
    m_neighbourRioterIDs.push_back(_ID);
}

void BaseGameEntity::clearAgentIDs()
{
    m_neighbourPoliceIDs.clear();
    m_neighbourRioterIDs.clear();
    m_neighbourObstacleIDs.clear();
}

