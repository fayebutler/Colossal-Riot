#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>
#include <cassert>
#include "BaseGameEntity.h"

class BaseGameEntity;

#define EntityMgr EntityManager::instance()


class EntityManager
{

public:

  static EntityManager* instance();

  template <typename Type>
  void registerEntity(Type* _newEntity)
  {
    m_entityMap.insert(std::make_pair(_newEntity->getID(), _newEntity));
  }

  //template <typename Type>
  void* getEntityFromID(int _ID)const
  {
    EntityMap::const_iterator entity = m_entityMap.find(_ID);

    assert ( (entity !=  m_entityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

    return (void*)entity->second;
  }

  template <typename Type>
  void removeEntity(Type* _entity)
  {
    m_entityMap.erase(m_entityMap.find(_entity->getID()));
  }

private:

  typedef std::map<int, void*> EntityMap;
  EntityMap m_entityMap;

  EntityManager(){}

  // DO THIS>>> MAYBE??!>!??!

  EntityManager(const EntityManager&);

  EntityManager& operator=(const EntityManager&);

};

#endif // ENTITYMANAGER_H
