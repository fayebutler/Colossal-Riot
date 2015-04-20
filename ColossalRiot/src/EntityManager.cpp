#include "EntityManager.h"
#include "BaseGameEntity.h"

EntityManager* EntityManager::instance()
{
  static EntityManager instance;

  return &instance;
}

//template <typename Type>
//BaseGameEntity *EntityManager::getEntityFromID(int _ID)const
//{
//  EntityMap::const_iterator entity = m_entityMap.find(_ID);

//  assert ( (entity !=  m_entityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

//  return (BaseGameEntity*)entity->second;
//}

//template <typename Type>
//void EntityManager::removeEntity(BaseGameEntity *_entity)
//{
//  m_entityMap.erase(m_entityMap.find(_entity->getID()));
//}

//template <typename Type>
//void EntityManager::registerEntity(Type *_newEntity)
//{
//  m_entityMap.insert(std::make_pair(_newEntity->getID(), _newEntity));
//}
