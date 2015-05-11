#include "EntityManager.h"
#include "BaseEntity.h"

EntityManager::EntityManager()
{
}

EntityManager* EntityManager::instance()
{
  static EntityManager instance;

  return &instance;
}


BaseEntity* EntityManager::getEntityFromID(int _ID)const
{
  EntityMap::const_iterator entity = m_entityMap.find(_ID);

  assert ( (entity !=  m_entityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

  return entity->second;
}


void EntityManager::removeEntity(BaseEntity* _entity)
{
  m_entityMap.erase(m_entityMap.find(_entity->getID()));
}


void EntityManager::registerEntity(BaseEntity* _newEntity)
{
  m_entityMap.insert(std::make_pair(_newEntity->getID(), _newEntity));
}
