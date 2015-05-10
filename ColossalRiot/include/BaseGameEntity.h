#ifndef BASEGAMEENTITY_H
#define BASEGAMEENTITY_H

#include <ngl/Transformation.h>
#include <vector>
#include <list>
#include "EntityManager.h"
#include "Message.h"
#include "MessageManager.h"
#include "Cell.h"

class GameWorld;
class EntityManager;
class MessageManager;

//----------------------------------------------------------------------------------------------------------------------
/// @file BaseGameEntity.h
/// @class BaseGameEntity "include/BaseGameEntity.h"
/// @brief The base class for all entities in the game, static or moving
/// @author Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// @brief enum to describe the type of entitiy
//----------------------------------------------------------------------------------------------------------------------
enum entityType
{
  typeMovingEntity, // 0
  typeStaticEntity, // 1
  typePolice,       // 2
  typeRioter,       // 3
  typeWall,         // 4
  typeSquad         // 5
};

class BaseGameEntity
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor this will create a base game entitiy in a valid world, with a type, position and radius, and register
  /// @brief it's unique ID with the Entity Manager
  /// @param[in] _world input world to create the base game entity in
  /// @param[in] _entityType input entity type to set for base game entity
  /// @param[in] _pos input world position vector to spawn the base game entity at
  /// @param[in] _r input radius to set as the bounding radius of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  BaseGameEntity(GameWorld* _world, entityType _entityType, ngl::Vec3 _pos, float _r);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor used to clean up the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  ~BaseGameEntity();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief virtual pure function to determin how any messages received by the base game entity should be handled
  /// @param[in] _message the message to be received by the base game entitiy
  //----------------------------------------------------------------------------------------------------------------------
  virtual bool handleMessage(const Message& _message) = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the ID of the base game entity
  /// @return int the value of the base game entity's ID
  //----------------------------------------------------------------------------------------------------------------------
  int getID() const { return m_ID; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the entity type of the base game entity
  /// @return enum entityType the enum type of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  entityType getEntityType() const { return m_entityType; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the entity type of the base game entity
  /// @param[in] _newType input to set the new type of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  void setEntityType(const entityType _newType) { m_entityType = _newType; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the position of the base game enitity in world space
  /// @return ngl::Vec3 the vector position of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getPos() const { return m_pos; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the position of the base game enitity in world space
  /// @param[in] _newPos input to set the new position of the base game entity in world space
  //----------------------------------------------------------------------------------------------------------------------
  void setPos(const ngl::Vec3 _newPos ) { m_pos = _newPos; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the bounding radius of the base game entity
  /// @return float the bounding radius of the base game enitity
  //----------------------------------------------------------------------------------------------------------------------
  float getBoundingRadius() const { return m_boundingRadius; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the bounding radius of the base game entitity
  /// @param[in] _r the new bouding radius of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  void setBoudingRadius(const float _r) { m_boundingRadius = _r; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the detection radius of the base game entity
  /// @return float the detection radius of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  float getDetectionRadius() const {return m_detectionRadius; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the detection radius of the base game entity
  /// @param[in] _r the new detection radius of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  void setDetectionRadius(const float _r) { m_detectionRadius = _r; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds the detected entity's ID to the vector of detected entity IDs
  /// @param[in] _ID the input ID of the detected entity to add
  //----------------------------------------------------------------------------------------------------------------------
  void addDetectedDynamicEntityID(int _ID) { m_detectedDynamicEntityIDs.push_back(_ID); }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clears all the IDs stored within the detected dynamic entity IDs vector
  //----------------------------------------------------------------------------------------------------------------------
  void clearDetectedDynamicEntityID() { m_detectedDynamicEntityIDs.clear(); }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the vector of all detected dynamic entitys IDs
  /// @return std::vector<int> the vector of all detected dynamic entity IDs
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> getDetectedEntityIDs() { return m_detectedDynamicEntityIDs; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the vector of all neighbour police IDs
  /// @return std::vector<int> the vector of all neighbour police IDs
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> getNeighbourPoliceIDs() { return m_neighbourPoliceIDs; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the vector of all neighbour rioter IDs
  /// @return std::vector<int> the vector of all neighbour rioter IDs
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> getNeighbourRioterIDs() { return m_neighbourRioterIDs; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the vector of all neighbour obstacle IDs
  /// @return std::vector<int> the vector of all neighbour obstacle IDs
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> getNeighbourObstacleIDs() { return m_neighbourObstacleIDs; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds the input police ID to the vector of neighbour police IDs
  /// @param[in] _ID input ID to add to vector of neighbour police IDs
  //----------------------------------------------------------------------------------------------------------------------
  void addPoliceID(int _ID) { m_neighbourPoliceIDs.push_back(_ID); }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds the input police ID to the vector of neighbour rioter IDs
  /// @param[in] _ID input ID to add to vector of neighbour rioter IDs
  //----------------------------------------------------------------------------------------------------------------------
  void addRioterID(int _ID) { m_neighbourRioterIDs.push_back(_ID); }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds the input police ID to the vector of neighbour obstacle IDs
  /// @param[in] _ID input ID to add to vector of neighbour obstacle IDs
  //----------------------------------------------------------------------------------------------------------------------
  void addObstacleID(int _ID) { m_neighbourObstacleIDs.push_back(_ID); }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clears all of the vectors of neighbour police/rioter/obstacle IDs
  //----------------------------------------------------------------------------------------------------------------------
  void clearAgentIDs() {     m_neighbourPoliceIDs.clear();
                             m_neighbourRioterIDs.clear();
                             m_neighbourObstacleIDs.clear(); }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the current cell that the base game entity in located in
  /// @return Cell the object of the cell that the base game entity is located in
  //----------------------------------------------------------------------------------------------------------------------
  Cell getCurrentCell() { return m_currentCell; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the current cell that the base game enitity is located in
  /// @param[in] _inCell the cell to set as the current cell
  //----------------------------------------------------------------------------------------------------------------------
  void setCurrentCell(const Cell _inCell) { m_currentCell = _inCell; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the ID of the current cell for the base game entity
  /// @return int the ID of the current cell for the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  int getCurrentCellID() { return m_currentCellID; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the current cell ID for the base game entity
  /// @param[in] _ID the ID of the input cell to set as current
  //----------------------------------------------------------------------------------------------------------------------
  void setCurrentCellID(const int _ID) { m_currentCellID = _ID; }

protected:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the world that the base game entity is contained in
  //----------------------------------------------------------------------------------------------------------------------
  GameWorld* m_world;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the entity manager that the base game entity is registered with
  //----------------------------------------------------------------------------------------------------------------------
  EntityManager* m_entityMgr;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the message manager that the base game entity uses to send messages
  //----------------------------------------------------------------------------------------------------------------------
  MessageManager* m_messageMgr;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the value of the base game entity's ID
  //----------------------------------------------------------------------------------------------------------------------
  int m_ID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the value of the next valid ID to be used by the entity manager
  //----------------------------------------------------------------------------------------------------------------------
  static int m_nextValidID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a method to set the ID of the base game entity for the entity manager
  //----------------------------------------------------------------------------------------------------------------------
  void setID(int _val) { m_ID = _val; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to reset the IDs when the game is restarted
  //----------------------------------------------------------------------------------------------------------------------
  bool m_resetID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the entity type of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  entityType m_entityType;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the position vector in world space of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_pos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the bouding radius of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  float m_boundingRadius;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the detection radius of the base game entity
  //----------------------------------------------------------------------------------------------------------------------
  float m_detectionRadius;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector of integer IDs for detected dynamic entities
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> m_detectedDynamicEntityIDs;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector of integer IDs for neighbour police IDs
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> m_neighbourPoliceIDs;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector of integer IDs for neighbour rioters IDs
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> m_neighbourRioterIDs;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector of ineteger IDs for neighbour obstacles
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> m_neighbourObstacleIDs;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the current cell that the base game entity is located in
  //----------------------------------------------------------------------------------------------------------------------
  Cell m_currentCell;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the integer ID of the current cell that the base game entity is located in
  //----------------------------------------------------------------------------------------------------------------------
  int m_currentCellID;
};

#endif // BASEGAMEENTITY_H
