#ifndef POLICE_H
#define POLICE_H

//----------------------------------------------------------------------------------------------------------------------------
/// @file Police.h
/// @brief defines a policeman entity
//----------------------------------------------------------------------------------------------------------------------------

#include "Agent.h"
#include "StateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
/// @class Police "include/Police.h"
/// @brief The base class for all policemen
/// @author Faye Butler
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

class Police : public Agent
{
public:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Police ctor
  /// @param[in] world: Pointer to the gameworld
  /// @param[in] _mesh: Pointer to the obj mesh used for a policeman
  //----------------------------------------------------------------------------------------------------------------------------
  Police(GameWorld* world, ngl::Obj *_mesh);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Police dtor
  //----------------------------------------------------------------------------------------------------------------------------
  ~Police();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the lua state of the policeman
  /// @return the current lua state
  //----------------------------------------------------------------------------------------------------------------------------
  lua_State* getLuaState() const { return L ; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to update each policeman at every tick
  /// @param[in] timeElapsed: the time elapsed between each tick
  /// @param[in] currentTime: the current time of the game
  //----------------------------------------------------------------------------------------------------------------------------
  void update(double timeElapsed, double currentTime);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to draw each policeman
  /// @param[in] cam: the camera used for the scene
  /// @param[in] mouseGlobalTX: the global mouse position transformation matrix
  //----------------------------------------------------------------------------------------------------------------------------
  void draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to load the matrices of the policeman to the shader
  /// @param[in] cam: the camera used for the scene
  /// @param[in] mouseGlobalTX: the global mouse position transformation matrix
  //----------------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the influence of the rioters
  /// @return Float: the amount of rioter influence
  //----------------------------------------------------------------------------------------------------------------------------
  float getRioterInfluence()const{return m_rioterInfluence;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to check that the policeman is within the pursuit range around the squad
  /// @param[in] _dist: the distance away from the squad the police can go
  //----------------------------------------------------------------------------------------------------------------------------
  void checkValidPursuitRange(float _dist);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to set the policeman on a path back to the police station
  //----------------------------------------------------------------------------------------------------------------------------
  void findPathHome();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the state machine for the policeman
  //----------------------------------------------------------------------------------------------------------------------------
  StateMachine<Police>* getStateMachine() const { return m_stateMachine; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to find a target to pursue
  /// @param[in] _health: the number to limit the search, if an agent is below this _health then it won't be found as a target
  //----------------------------------------------------------------------------------------------------------------------------
  void findTargetID(float _health);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to define how to handle messages
  /// @param[in] _messgae:message recieved by the policeman
  //----------------------------------------------------------------------------------------------------------------------------
  bool handleMessage(const Message &_message);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to send a message to attack (deliver damage to target)
  //----------------------------------------------------------------------------------------------------------------------------
  void attack();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to send message if a policeman has died
  //----------------------------------------------------------------------------------------------------------------------------
  void death();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to register class, functions and properties to be used in lua
  /// @param[in] _L: the lua state
  //----------------------------------------------------------------------------------------------------------------------------
  void registerClass(lua_State* _L);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the position of the squad
  /// @return Vec3: vector of the position of the squad
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getSquadPos()const{return m_squadPos;}
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the position of the squad
  /// @param[in] newPos: vector to set the position of the squad
  //----------------------------------------------------------------------------------------------------------------------------
  void setSquadPos(ngl::Vec3 newPos) {m_squadPos = newPos;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the radius of the policeman's squad
  /// @return Float: the radius of the policeman's squad
  //----------------------------------------------------------------------------------------------------------------------------
  float getSquadRadius()const{return m_squadRadius;}
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the the radius of the policeman's squad
  /// @param[in] newRad: number to set the radius of the policeman's squad to
  //----------------------------------------------------------------------------------------------------------------------------
  void setSquadRadius(const float newRad) {m_squadRadius = newRad;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to set the squad cohesion of the policeman's squad
  /// @param[in] weight: the weight to set squad cohesion to
  //----------------------------------------------------------------------------------------------------------------------------
  void squadCohesion(double weight);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the policeman's squad ID
  /// @return Int: the number of the policeman's squad ID
  //----------------------------------------------------------------------------------------------------------------------------
  int getSquadID()const{return m_squadID;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the policeman's squad ID
  /// @param[in] squadID: number to set the policeman's squad ID to
  //----------------------------------------------------------------------------------------------------------------------------
  void setSquadID(const int squadID){m_squadID = squadID;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets whether the policeman is moving (move state in lua)
  /// @return Boolean: true if the policeman is in move state, false if it is not
  //----------------------------------------------------------------------------------------------------------------------------
  bool getIsMoving() const { return m_isMoving; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets if the policeman is moving (move state in lua)
  /// @param[in] _val: boolean, set to true if the policeman is moving, false if not
  //----------------------------------------------------------------------------------------------------------------------------
  void setIsMoving(const bool _val) { m_isMoving = _val; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the blockade position of the policeman
  /// @return Vec3: vector for the position of the policeman's blockade position
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getBlockadePos()const{return m_blockadePosition;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the blockade position of the policeman
  /// @param[in] newBlockadePos: the vector to set the policeman's blockade position to
  //----------------------------------------------------------------------------------------------------------------------------
  void setBlockadePos(const ngl::Vec3 newBlockadePos) {m_blockadePosition = newBlockadePos;}

protected:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the message manager
  //----------------------------------------------------------------------------------------------------------------------------
  MessageManager* m_messageMgr;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the state machine
  //----------------------------------------------------------------------------------------------------------------------------
  StateMachine<Police>* m_stateMachine;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief vector to contain the current policeman's squad position
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_squadPos;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief float to hold the policeman's squad radius
  //----------------------------------------------------------------------------------------------------------------------------
  float m_squadRadius;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief number of the policeman's squad ID
  //----------------------------------------------------------------------------------------------------------------------------
  int m_squadID;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Boolean to hold whether the squad is moving or not
  //----------------------------------------------------------------------------------------------------------------------------
  bool m_isMoving;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Boolean to hold whether the policeman is on a valid pursuit
  //----------------------------------------------------------------------------------------------------------------------------
  bool m_validPursuit;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector to hold the policeman's blockade position
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_blockadePosition;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Float to hold the amount of rioter influence
  //----------------------------------------------------------------------------------------------------------------------------
  float m_rioterInfluence;

};

#endif // POLICE_H
