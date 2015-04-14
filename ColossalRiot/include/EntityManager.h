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

  void registerEntity(BaseGameEntity* _newEntity);
  BaseGameEntity* getEntityFromID(int _ID) const;

  void removeEntity(BaseGameEntity* _entity);


private:
  typedef std::map<int, BaseGameEntity*> EntityMap;
  EntityMap m_entityMap;

  EntityManager();

  EntityManager(const EntityManager&);
  EntityManager& operator=(const EntityManager&);
};

#endif // ENTITYMANAGER_H
