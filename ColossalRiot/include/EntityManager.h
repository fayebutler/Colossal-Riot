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

  static EntityManager* instance()
  {
    static EntityManager instance;

    return &instance;
  }

  template <typename Type>
  void registerEntity(Type* _newEntity)
  {
    m_entityMap.insert(std::make_pair(_newEntity->getID(), _newEntity));
  }


  BaseGameEntity* getEntityFromID(int _ID)const
  {
    EntityMap::const_iterator entity = m_entityMap.find(_ID);

    assert ( (entity !=  m_entityMap.end()) && "<EntityManager::GetEntityFromID>: invalid ID");

    return (BaseGameEntity*)entity->second;
  }

  template <typename Type>
  void removeEntity(Type* _entity)
  {
    m_entityMap.erase(m_entityMap.find(_entity->getID()));
  }

  int getSize()
  {
      return m_entityMap.size();
  }

//  typedef std::map<int, BaseGameEntity*> EntityMap;
//  EntityMap m_entityMap;


private:

  typedef std::map<int, BaseGameEntity*> EntityMap;
  EntityMap m_entityMap;

  EntityManager(){}

  EntityManager(const EntityManager&);

  EntityManager& operator=(const EntityManager&);

};

#endif // ENTITYMANAGER_H
