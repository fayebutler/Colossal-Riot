#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>
#include <cassert>
#include "BaseEntity.h"

class BaseEntity;

#define EntityMgr EntityManager::instance()


class EntityManager
{
public:
  static EntityManager* instance();

  void registerEntity(BaseEntity* _newEntity);
  BaseEntity* getEntityFromID(int _ID) const;

  void removeEntity(BaseEntity* _entity);


private:
  typedef std::map<int, BaseEntity*> EntityMap;
  EntityMap m_entityMap;

  EntityManager();

  EntityManager(const EntityManager&);
  EntityManager& operator=(const EntityManager&);
};

#endif // ENTITYMANAGER_H
