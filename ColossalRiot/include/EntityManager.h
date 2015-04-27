#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <map>
#include <cassert>
#include "BaseGameEntity.h"


class BaseGameEntity;

class EntityManager
{

private:

  typedef std::map<int, BaseGameEntity*> EntityMap;
  static EntityMap m_entityMap;

public:

  EntityManager();
  ~EntityManager();

  //template <typename Type>
  void registerEntity(BaseGameEntity* _newEntity);

  EntityMap getEntityMap();

  BaseGameEntity* getEntityFromID(int _ID) const;

  //template <typename Type>
  void removeEntity(BaseGameEntity* _entity);

  int getSize();
};

#endif // ENTITYMANAGER_H
