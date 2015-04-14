#include "EntityManager.h"
#include "BaseGameEntity.h"

EntityManager::EntityManager()
{
}

EntityManager* EntityManager::instance()
{
  static EntityManager instance;

  return &instance;
}


BaseGameEntity* EntityManager::getEntityFromID(int _ID)const
{
  EntityMap::const_iterator entity = m_entityMap.find(_ID);

  assert ( (entity !=  m_entityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

  return entity->second;
}


void EntityManager::removeEntity(BaseGameEntity* _entity)
{
  m_entityMap.erase(m_entityMap.find(_entity->getID()));
}


void EntityManager::registerEntity(BaseGameEntity* _newEntity)
{
  m_entityMap.insert(std::make_pair(_newEntity->getID(), _newEntity));
}
