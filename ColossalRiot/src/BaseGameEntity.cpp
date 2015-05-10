//----------------------------------------------------------------------------------------------------------------------
/// @file BaseGameEntity.cpp
/// @brief The base class for all entites in the game, static or moving
//----------------------------------------------------------------------------------------------------------------------

#include "BaseGameEntity.h"
#include "GameWorld.h"

//----------------------------------------------------------------------------------------------------------------------
/// @brief the starting valid ID to be used
//----------------------------------------------------------------------------------------------------------------------
int BaseGameEntity::m_nextValidID = 0;

//----------------------------------------------------------------------------------------------------------------------
BaseGameEntity::BaseGameEntity(GameWorld* _world, entityType _entityType, ngl::Vec3 _pos, float _r)
{
  m_world = _world;
  if(m_world->getResetID()==1)
  {
    m_nextValidID = 0;
    m_world->setResetID(0);
  }

  setID(m_nextValidID);
  m_nextValidID++;

  m_entityMgr = new EntityManager();
  m_entityMgr->registerEntity(this);

  m_pos = _pos;
  m_boundingRadius = _r;
  m_entityType = _entityType;
  m_detectionRadius = _r * 3; // CHANGE TO VARIABLE

  m_world->getCellGraph()->initializeCells(this);
}

//----------------------------------------------------------------------------------------------------------------------
BaseGameEntity::~BaseGameEntity()
{
    //delete m_entityMgr;
}

//----------------------------------------------------------------------------------------------------------------------
bool BaseGameEntity::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {
    default:
    {
      std::cout<<"BaseGameEntity: Message type not defined"<<std::endl;
      return false;
    }
  }
}

