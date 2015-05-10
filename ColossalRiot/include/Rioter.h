#ifndef RIOTER_H
#define RIOTER_H

//----------------------------------------------------------------------------------------------------------------------------
/// @file Rioter.h
/// @brief defines a rioter entity
//----------------------------------------------------------------------------------------------------------------------------

#include "Agent.h"
#include "StateMachine.h"

//----------------------------------------------------------------------------------------------------------------------
/// @class Rioter "include/Rioter.h"
/// @brief The base class for all rioters
/// @author Faye Butler
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

class Rioter : public Agent
{
public:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Rioter ctor
  /// @param[in] world: Pointer to the gameworld
  /// @param[in] _mesh: Pointer to the obj mesh for a rioter
  //----------------------------------------------------------------------------------------------------------------------------
  Rioter(GameWorld* world, ngl::Obj* _mesh);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Rioter dtor
  //----------------------------------------------------------------------------------------------------------------------------
  ~Rioter();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the lua state of the rioters
  /// @return the current lua state
  //----------------------------------------------------------------------------------------------------------------------------
  lua_State* getLuaState() const { return L ; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to update the rioter at every tick
  /// @param[in] timeElapsed: the time elapsed since the last tick
  /// @param[in] currentTime: the current time of the game
  //----------------------------------------------------------------------------------------------------------------------------
  void update(double timeElapsed, double currentTime);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to draw the rioters
  /// @param[in] cam: the camera used in the scene
  /// @param[in] mouseGlobalTX: the global mouse position transformation matrix
  //----------------------------------------------------------------------------------------------------------------------------
  void draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to load the matrices of the rioter to the shader
  /// @param[in] cam: the camera used for the scene
  /// @param[in] mouseGlobalTX: the global mouse position transformation matrix
  //----------------------------------------------------------------------------------------------------------------------------
  void loadMatricesToShader(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the state machine of the rioter
  /// @return the rioters state machine
  //----------------------------------------------------------------------------------------------------------------------------
  StateMachine<Rioter>* getStateMachine() const { return m_stateMachine; }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to find a target to pursue
  /// @param[in] _health: the number to limit the search, if an agent is below this _health then it won't be found as a target
  //----------------------------------------------------------------------------------------------------------------------------
  void findTargetID(float _health);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to define how to handle messages
  /// @param[in] _messgae:message recieved by the rioter
  //----------------------------------------------------------------------------------------------------------------------------
  bool handleMessage(const Message &_message);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to send message to attack (deliver damage to target)
  //----------------------------------------------------------------------------------------------------------------------------
  void attack();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to send message if a rioter has died
  //----------------------------------------------------------------------------------------------------------------------------
  void death();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to set the rioter on a path to an exit
  //----------------------------------------------------------------------------------------------------------------------------
  void findPathHome();

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to set the protest cohesion of the rioter
  /// @param[in] weight: the weight to set the cohesion to
  //----------------------------------------------------------------------------------------------------------------------------
  void protestCohesion(double weight);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Method to register class, functions and properties to be used in lua
  /// @param[in] _L: the lua state
  //----------------------------------------------------------------------------------------------------------------------------
  void registerClass(lua_State* _L);

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the influence of the   police
  /// @return Float: the amount of police influence
  //----------------------------------------------------------------------------------------------------------------------------
  float getPoliceInfluence()const{return m_policeInfluence;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Get the protest position
  /// @return Vec3: the vector of the protest position
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getProtestPos()const{return m_protestPos;}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Set the protest position
  /// @param[in] newPos: vector of the new protest position
  //----------------------------------------------------------------------------------------------------------------------------
  void setProtestPos(ngl::Vec3 newPos) {m_protestPos = newPos;}

protected:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the message manager
  //----------------------------------------------------------------------------------------------------------------------------
  MessageManager* m_messageMgr;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the state machine
  //----------------------------------------------------------------------------------------------------------------------------
  StateMachine<Rioter>* m_stateMachine;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Vector to contain the protest position
  //----------------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_protestPos;

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Float to hold the amount of police influence
  //----------------------------------------------------------------------------------------------------------------------------
  float m_policeInfluence;

};

#endif // RIOTER_H
