#include "EntityManager.h"

typedef std::map<int, BaseGameEntity*> EntityMap;
EntityMap EntityManager::m_entityMap;

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

//template <typename Type>
void EntityManager::registerEntity(BaseGameEntity* _newEntity)
{
  EntityManager::m_entityMap.insert(std::make_pair(_newEntity->getID(), _newEntity));
}

EntityManager::EntityMap EntityManager::getEntityMap()
{
  return m_entityMap;
}

BaseGameEntity* EntityManager::getEntityFromID(int _ID)const
{
  EntityMap::const_iterator entity = m_entityMap.find(_ID);

  assert ( (entity !=  m_entityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID ");

  return (BaseGameEntity*)entity->second;
}

//template <typename Type>
void EntityManager::removeEntity(BaseGameEntity* _entity)
{
  m_entityMap.erase(m_entityMap.find(_entity->getID()));
}

int EntityManager::getSize()
{
    return m_entityMap.size();
}
