#ifndef GAMEWORLD_H
#define GAMEWORLD_H

//----------------------------------------------------------------------------------------------------------------------
/// @file GameWorld.h
/// @class GameWorld "include/GameWorld.h"
/// @brief The class that creates the world an encompasses all base game entities
/// @author Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include "Rioter.h"
#include "Police.h"
#include "CellGraph.h"
#include "Squad.h"
#include "StaticEntity.h"
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}
#include "LuaBridge.h"

typedef std::vector<BaseGameEntity*>::iterator ObIt;

class GameWorld
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor this will create a new world to spawn entities inside
  /// @param[in] _level the level to start as specified by the UI button and Lua script
  //----------------------------------------------------------------------------------------------------------------------
  GameWorld(int _level);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor cleans up all entities in the world and closes the Lua state associated with the level
  //----------------------------------------------------------------------------------------------------------------------
  ~GameWorld();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief registers all the necessary functions and members with Lua so that they can be edited in our script file
  /// @param[in] _L the Lua state to register with
  //----------------------------------------------------------------------------------------------------------------------
  void registerLua(lua_State *_L);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief updates all of the entities in the world, the cells in the cell graph and checks for change in state
  /// @param[in] _timeElapsed the time elapsed since last tick
  /// @param[in] _currentTime the current time of the level
  //----------------------------------------------------------------------------------------------------------------------
  void Update(double _timeElapsed, double _currentTime);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to draw the game world and contained entities
  /// @param[in] _cam the camera used for the scene
  /// @param[in] _mouseGlobalTX the global mouse position transformation matrix
  //----------------------------------------------------------------------------------------------------------------------
  void draw(ngl::Camera *_cam, ngl::Mat4 _mouseGlobalTX);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method to load the matrices of the game world to the shader
  /// @param[in] _cam the camera used for the scene
  /// @param[in] _mouseGlobalTX the global mouse position transformation matrix
  //----------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader(ngl::Camera *_cam, ngl::Mat4 _mouseGlobalTX);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets if the player has achieved the win conditions
  /// @return bool returns if the win state has been achieved (true) or not (false)
  //----------------------------------------------------------------------------------------------------------------------
  bool getHasWon() const { return m_win; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets that the player has achieved the win state or not
  /// @param[in] _inBool the boolean to assign to the win state
  //----------------------------------------------------------------------------------------------------------------------
  void setHasWon(const bool _inBool) { m_win = _inBool; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets if the player has achieved the lose conditions
  /// @return bool returns if the lose state has been achieved (true) or not (false)
  //----------------------------------------------------------------------------------------------------------------------
  bool getHasLost() const { return m_lose; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets that the play has achieved the lose state or not
  /// @param[in] _inBool the boolean to assign to the lose state
  //----------------------------------------------------------------------------------------------------------------------
  void setHasLost(const bool _inBool) { m_lose = _inBool; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets if the reset ID is true or false
  /// @return bool returns the value of of the reset ID
  //----------------------------------------------------------------------------------------------------------------------
  bool getResetID() const { return m_resetID; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets if the reset ID is true or false
  /// @param[in] _resetID the boolean to assign to the reset ID
  //----------------------------------------------------------------------------------------------------------------------
  void setResetID(const bool _resetID) { m_resetID = _resetID; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the vector of all rioters in the current game world
  /// @return std::vector<Rioter*> returns a vector of rioters in game world
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Rioter*> getRioters() const { return m_rioters; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gest the number of rioters in the game world
  /// @return int returns the number of rioters in the game world
  //----------------------------------------------------------------------------------------------------------------------
  int getNumberOfRioters() const { return m_numberOfRioters; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the initial number of rioters that were spawned in the level
  /// @return int returns the initial number of rioters that were spawned in the level
  //----------------------------------------------------------------------------------------------------------------------
  int getInitialNumberOfRioters() const { return m_initialNumberOfRioters; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the initial number of rioters that were spawned in the level
  /// @param[in] _number amount to set as the initial number of rioters
  //----------------------------------------------------------------------------------------------------------------------
  void setInitialNumberOfRioters(const int _number) { m_initialNumberOfRioters = _number; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the number of rioters who have died during this level
  /// @return int returns the number of rioters who have died during this level
  //----------------------------------------------------------------------------------------------------------------------
  int getNumberOfRiotersDead() const { return m_numberOfRiotersDead; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the number of rioters who have died during this level
  /// @param[in] _number amount to set as the number of dead rioters
  //----------------------------------------------------------------------------------------------------------------------
  void setNumberOfRiotersDead(const int _number) { m_numberOfRiotersDead = _number; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the number of rioters who have gone home during this level
  /// @return int returns the number of rioters who have gone home during this level
  //----------------------------------------------------------------------------------------------------------------------
  int getNumberOfRiotersHome() const { return m_numberOfRiotersHome; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the number of rioters that have gone home during this level
  /// @param[in] _number amount to set as the number of rioters who have gone home
  //----------------------------------------------------------------------------------------------------------------------
  void setNumberOfRiotersHome(const int _number) { m_numberOfRiotersHome = _number; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the number of rioters needed for the lose state to be achieved
  /// @return int returns the number of rioters needed for the lose state to be achieved
  //----------------------------------------------------------------------------------------------------------------------
  int getNumberOfRiotersDeadToLose() const { return m_numberOfRiotersDeadToLose; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the number of rioters needed for the lose state to be achieved
  /// @param[in] _number value to set as the number of dead rioters needed to achieve the lose state
  //----------------------------------------------------------------------------------------------------------------------
  void setNumberOfRiotersDeadToLose(const int _number) { m_numberOfRiotersDeadToLose = _number; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the number of rioters needed for the win state to be achieved
  /// @return int returns the number of rioters needed for the win staete to achieved
  //----------------------------------------------------------------------------------------------------------------------
  int getNumberOfRiotersHomeToWin() const { return m_numberOfRiotersHomeToWin; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the number of rioters needed for the win state to be achieved
  /// @param[in] _number value to set as the number of rioter needed to have gone home to achieve the win state
  //----------------------------------------------------------------------------------------------------------------------
  void setNumberOfRiotersHomeToWin(const int _number) { m_numberOfRiotersHomeToWin = _number; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief creates a squad of a specific size
  /// @param[in] _size the input size of the squad
  //----------------------------------------------------------------------------------------------------------------------
  void createSquad(int _size);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the vector of all squads in the current level
  /// @return std::vector<Squad*> returns the vector of all squads in the level
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Squad*> getSquads() { return m_squads; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the target position for the currently selected squad
  /// @param[in] _selectedSquad the currently selected squad
  /// @param[in] _the target position to be given to the squad
  //----------------------------------------------------------------------------------------------------------------------
  void squadTarget(Squad *_selectedSquad, ngl::Vec3 _target);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the number of available police during the current level
  /// @return int returns the number of available police during the current level
  //----------------------------------------------------------------------------------------------------------------------
  int getAvailablePolice() const { return m_availablePolice; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the number of available police during the current level
  /// @param[in] _availablePolice value to set as the number of available police
  //----------------------------------------------------------------------------------------------------------------------
  void setAvailablePolice(const int _availablePolice) { m_availablePolice = _availablePolice; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the cell graph generated for the current level
  /// @returns CellGraph* returns the cell graph generated for the current level
  //----------------------------------------------------------------------------------------------------------------------
  CellGraph* getCellGraph() const { return m_cellGraph; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the file used for generating the cell graph
  /// @return char* returns the file used for generating the cell graph
  //----------------------------------------------------------------------------------------------------------------------
  const char* getCellGraphFile() const { return m_cellGraphFile; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the file to be used for generating the cell graph
  /// @param[in] _file the input file to be used for generating the cell graph
  //----------------------------------------------------------------------------------------------------------------------
  void setCellGraphFile(const char* _file) { m_cellGraphFile = _file;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the file used for the street mesh
  /// @return std::string returns the file used for the street mesh
  //----------------------------------------------------------------------------------------------------------------------
  std::string getStreetMeshFile() const { return m_streetMeshFile; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the file to be used for the street mesh
  /// @param[in] _file the input file to be used for generating the street mesh
  //----------------------------------------------------------------------------------------------------------------------
  void setStreetMeshFile(const std::string _file) { m_streetMeshFile = _file; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the file used for the building mesh
  /// @return std::string returns the file used for the building mesh
  //----------------------------------------------------------------------------------------------------------------------
  std::string getBuildingMeshFile() const { return m_buildingMeshFile; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the file to be used for the building mesh
  /// @param[in] _file the input file to be used for the building mesh
  //----------------------------------------------------------------------------------------------------------------------
  void setBuildingMeshFile(const std::string _file) { m_buildingMeshFile = _file; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method exposed to Lua to add a protest position for rioters to gather around
  /// @param[in] _x input x world position for the protest position
  /// @param[in] _y input y world position for the protest position
  /// @param[in] _z input z world position for the protest position
  //----------------------------------------------------------------------------------------------------------------------
  void addProtestPosition(float _x, float _y, float _z);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the vector that stores all the protests positions that have been added
  /// @return std::vector<ngl::Vec3> returns the vector that stores the protest positions
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> getProtestPositions() const { return m_protestPositions; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the world position of the police station
  /// @return ngl::Vec3 returns the world prosition of the police station
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getPoliceStation() const { return m_policeStation; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief method exposed to Lua that sets the world position of the police state
  /// @param[in] _x input x world position for the police station
  /// @param[in] _y input y world position for the police station
  /// @param[in] _z input z world position for the police station
  //----------------------------------------------------------------------------------------------------------------------
  void setPoliceStation(const float _x, const float _y, const float _z);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the number of trees spawned in the level
  /// @return int returns the number of trees spawn in the level
  //----------------------------------------------------------------------------------------------------------------------
  int getNumberOfTrees() const { return m_numberOfTrees; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the number of trees spawned in the level
  /// @param[in] _number input number to assign to number of spawned trees
  //----------------------------------------------------------------------------------------------------------------------
  void setNumberOfTrees(const int _number) {m_numberOfTrees = _number; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the number of street lights spawned in the level
  /// @return in returns the number of street lights spawned in the level
  //----------------------------------------------------------------------------------------------------------------------
  int getNumberOfStreetLights() const { return m_numberOfStreetLights; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the number of street lights spawned in the level
  /// @param[in] _number input number to assign to the number of spawned street lights
  //----------------------------------------------------------------------------------------------------------------------
  void setNumberOfStreetLights(const int _number) {m_numberOfStreetLights = _number; }

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the entity manager monostate
  //----------------------------------------------------------------------------------------------------------------------
  EntityManager* m_entityMgr;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the Lua state that is registers with the game world
  //----------------------------------------------------------------------------------------------------------------------
  lua_State *L;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a boolean that is set to true when the game is reset
  //----------------------------------------------------------------------------------------------------------------------
  bool m_resetID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a boolean to determin if the win state has been achieved
  //----------------------------------------------------------------------------------------------------------------------
  bool m_win;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a boolean to determin if the lose state has been achieved
  //----------------------------------------------------------------------------------------------------------------------
  bool m_lose;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector containing all of the dynamic entites found within the world
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> m_dynamicEntities;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the mesh that will be used by all rioters in the world
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Obj *m_rioterMesh;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector containing all of the rioters found within the world
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Rioter*> m_rioters;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the value of the initial number of rioters spawned at the start of the level
  //----------------------------------------------------------------------------------------------------------------------
  int m_initialNumberOfRioters;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the value of the number of rioters currently in the level
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfRioters;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector containing all the dead rioters to be used later or discarded
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Rioter*> m_deadRioters;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of dead rioters in the world
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfRiotersDead;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of rioters who have gone home during the game
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfRiotersHome;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of rioters who have to die for the player to lose
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfRiotersDeadToLose;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of rioters who have to go home for the player to win
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfRiotersHomeToWin;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector containing all of the squads currently in the level
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Squad*> m_squads;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of squads currently in the level
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfSquads;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of dead squads in currently in the level
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Squad*> m_deadSquads;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the mesh that will be used by all police in the world
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Obj *m_policeMesh;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of available police in the level
  //----------------------------------------------------------------------------------------------------------------------
  int m_availablePolice;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of active police in the level
  //----------------------------------------------------------------------------------------------------------------------
  int m_activePolice;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the mesh that will be used by all trees in the world
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Obj *m_treeMesh;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the mesh that will be used by all street lights in the world
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Obj *m_streetLightMesh;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector containing all of the obstacles currently in the level
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<StaticEntity*> m_obstacles;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of obstacles currently in the level
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfObstacles;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of trees currently in the level
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfTrees;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the number of street lights currently in the level
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfStreetLights;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the cell graph used by the world to generate the cell space partitioning
  //----------------------------------------------------------------------------------------------------------------------
  CellGraph* m_cellGraph;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the char* location and name of the file to be used by the cell graph
  //----------------------------------------------------------------------------------------------------------------------
  const char* m_cellGraphFile;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the mesh to be used by the street in the level
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Obj *m_streetMesh;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the string location and name of the street mesh
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_streetMeshFile;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the mesh to be used by the building in the level
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Obj *m_buildingMesh;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the string location and name of the building mesh
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_buildingMeshFile;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the location of the police station where new squads are spawned
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_policeStation;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a vector containing all of the position where rioters gather when protesting
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> m_protestPositions;
};

#endif // GAMEWORLD_H
