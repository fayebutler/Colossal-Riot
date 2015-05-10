#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

//----------------------------------------------------------------------------------------------------------------------
/// @file EntityManager.h
/// @class EntityManager "include/EntityManager.h"
/// @brief A manager monostate class that stores all base game entities with a corresponding ID number
/// @author Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

#include <map>
#include "BaseGameEntity.h"
#include <iostream>

class BaseGameEntity;

typedef std::map<int, BaseGameEntity*> EntityMap;

class EntityManager
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor this will create a default entitiy manager
  //----------------------------------------------------------------------------------------------------------------------
  EntityManager();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor this will destruct the entity manager
  //----------------------------------------------------------------------------------------------------------------------
  ~EntityManager();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief inserts an entity into the entity map along with it's unique ID
  /// @param[in] _newEntity the entity to be registered with the entity map
  //----------------------------------------------------------------------------------------------------------------------
  void registerEntity(BaseGameEntity* _newEntity);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the entity map
  /// @return EntityMap map of entities and corresponding IDs
  //----------------------------------------------------------------------------------------------------------------------
  EntityMap getEntityMap() { return s_entityMap; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the size of the entity map
  /// @return int value of the size of the entity map
  //----------------------------------------------------------------------------------------------------------------------
  int getSize() { return s_entityMap.size(); }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets a specific base game entity from the entity map based on input ID
  /// param[in] _ID input ID to specify which entity to get
  //----------------------------------------------------------------------------------------------------------------------
  BaseGameEntity* getEntityFromID(int _ID) const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief removes a specific entity from the entity map
  /// @param[in] _entity input the specific base game entity object to be deleted
  //----------------------------------------------------------------------------------------------------------------------
  void removeEntity(BaseGameEntity* _entity);
  void clearMap();

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the map used to store all of the base game entities and their corresponding unique ID
  //----------------------------------------------------------------------------------------------------------------------
  static EntityMap s_entityMap;
};

#endif // ENTITYMANAGER_H
