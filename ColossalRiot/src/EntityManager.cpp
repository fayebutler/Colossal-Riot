//----------------------------------------------------------------------------------------------------------------------
/// @file EntityManager.cpp
/// @brief A manager monostate class that stores all base game entities with a corresponding ID number
//----------------------------------------------------------------------------------------------------------------------

#include "EntityManager.h"

EntityMap EntityManager::s_entityMap;

//----------------------------------------------------------------------------------------------------------------------
EntityManager::EntityManager()
{
}

//----------------------------------------------------------------------------------------------------------------------
EntityManager::~EntityManager()
{
}

//----------------------------------------------------------------------------------------------------------------------
void EntityManager::registerEntity(BaseGameEntity* _newEntity)
{
  EntityManager::s_entityMap.insert(std::make_pair(_newEntity->getID(), _newEntity));
}

//----------------------------------------------------------------------------------------------------------------------
BaseGameEntity* EntityManager::getEntityFromID(int _ID)const
{
  EntityMap::const_iterator entity = s_entityMap.find(_ID);
  assert ( (entity !=  s_entityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID ");
  return (BaseGameEntity*)entity->second;
}

//----------------------------------------------------------------------------------------------------------------------
void EntityManager::removeEntity(BaseGameEntity* _entity)
{
  s_entityMap.erase(s_entityMap.find(_entity->getID()));
}

void EntityManager::clearMap()
{
    s_entityMap.clear();
}
