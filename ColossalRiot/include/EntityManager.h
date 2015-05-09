#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>
#include "BaseGameEntity.h"
#include <iostream>


class BaseGameEntity;

class EntityManager
{

private:

  typedef std::map<int, BaseGameEntity*> EntityMap;
  static EntityMap m_entityMap;

public:

  EntityManager();
  ~EntityManager();

  void registerEntity(BaseGameEntity* _newEntity);

  EntityMap getEntityMap();

  BaseGameEntity* getEntityFromID(int _ID) const;

  void removeEntity(BaseGameEntity* _entity);

  int getSize();
};

#endif // ENTITYMANAGER_H
