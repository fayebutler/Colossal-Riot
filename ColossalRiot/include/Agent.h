#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include "Vehicle.h"
#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Obj.h>
#include <Timer.h>
extern "C"
{
  #include <lua.h>
  #include <lauxlib.h>
  #include <lualib.h>
}
#include "LuaBridge.h"

//----------------------------------------------------------------------------------------------------------------------
/// @file Agent.h
/// @class Agent "include/Agent.h"
/// @brief The base class for all agents, that can be rioters or police
/// @author Will Herbert
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

class Agent: public Vehicle
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor this will create a valid Lua state and initialise into a valid world
  /// @param[in] _world input world to create the agent in
  //----------------------------------------------------------------------------------------------------------------------
  Agent(GameWorld *_world);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor used to remove agent and close Lua state
  //----------------------------------------------------------------------------------------------------------------------
  ~Agent();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief virtual pure method to update the agent every tick
  /// @param[in] _timeElapsed the time elapsed between each tick
  /// @param[in] _currentTime the current time of the level
  //----------------------------------------------------------------------------------------------------------------------
  virtual void update(double timeElapsed, double currentTime) = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief determins how any messages received by the agent should be handled
  /// @param[in] _message the message to be received by the agent
  //----------------------------------------------------------------------------------------------------------------------
  virtual bool handleMessage(const Message& _message);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief registers the agent's functions and properties with the valid Lua state
  /// @param[in] _L input the Lua state to register functions with
  //----------------------------------------------------------------------------------------------------------------------
  void registerLua(lua_State *_L);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to expose the function to Lua, so wander steering behaviour can be turned on and off, with a weight
  /// @param[in] _weight input to determin the weight of the wander steering behaviour, 0 turns off the behaviour
  //----------------------------------------------------------------------------------------------------------------------
  void wander(double _weight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to expose the function to Lua, so pursuit steering behaviour can be turned on and off, with a weight
  /// @param[in] _weight input to determin the weight of the pursuit steering behaviour, 0 turns off the behaviour
  //----------------------------------------------------------------------------------------------------------------------
  void pursuit(double _weight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to expose the function to Lua, so evade steering behaviour can be turned on and off, with a weight
  /// @param[in] _weight input to determin the weight of the evade steering behaviour, 0 turns off the behaviour
  //----------------------------------------------------------------------------------------------------------------------
  void evade(double _weight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to expose the function to Lua, so seek steering behaviour can be turned on and off, with a weight
  /// @param[in] _weight input to determin the weight of the seek steering behaviour, 0 turns off the behaviour
  //----------------------------------------------------------------------------------------------------------------------
  void seek(double _weight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to expose the function to Lua, so arrive steering behaviour can be turned on and off, with a weight
  /// @param[in] _weight input to determin the weight of the arrive steering behaviour, 0 turns off the behaviour
  //----------------------------------------------------------------------------------------------------------------------
  void arrive(double _weight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to expose the function to Lua, so cohesion steering behaviour can be turned on and off, with a weight
  /// @param[in] _weight input to determin the weight of the cohesion steering behaviour, 0 turns off the behaviour
  //----------------------------------------------------------------------------------------------------------------------
  void cohesion(double _weight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to expose the function to Lua, so separation steering behaviour can be turned on and off, with a weight
  /// @param[in] _weight input to determin the weight of the separation steering behaviour, 0 turns off the behaviour
  //----------------------------------------------------------------------------------------------------------------------
  void separation(double _weight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used to expose the function to Lua, so alignment steering behaviour can be turned on and off, with a weight
  /// @param[in] _weight input to determin the weight of the alignment steering behaviour, 0 turns off the behaviour
  //----------------------------------------------------------------------------------------------------------------------
  void alignment(double _weight);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the ID of the agent's current target
  /// @return int value of the agent's current target
  //----------------------------------------------------------------------------------------------------------------------
  int getTargetID() const { return m_targetID; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the health of the agent
  /// @param[in] _val input to set the agent's target ID to
  //----------------------------------------------------------------------------------------------------------------------
  void setTargetID(const int _val);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pure virtual method to find a target ID with highest rage with minimum health threshold
  /// @param[in] _health input to determin the minimum health to consider
  //----------------------------------------------------------------------------------------------------------------------
  virtual void findTargetID(float _health) = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief checks if the target is valid, are close enough to the agent, and if they have enough health
  /// @param[in] _dist input to determin maximum distance to valid target
  /// @param[in] _health intput to determin minimum health to consider
  //----------------------------------------------------------------------------------------------------------------------
  void checkValidTarget(float _dist, float _health);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief determins if the target agent is within reach
  /// @param[in] _reach the input of how far the agent can reach
  //----------------------------------------------------------------------------------------------------------------------
  bool targetWithinReach(float _reach);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the position of the agent's home
  /// @return ngl::Vec3 value of the agent's home
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getHomePos() const { return m_homePos; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the position of the agent's home
  /// @param[in] _val input to set the agent's home position
  //----------------------------------------------------------------------------------------------------------------------
  void setHomePos(const ngl::Vec3 _val) { m_homePos = _val; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets if the agent has a valid path home
  /// @return boolean true if it has a valid path, false if it does not have a valid path
  //----------------------------------------------------------------------------------------------------------------------
  bool getHasPathHome() const { return m_hasPathHome; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets if the agent has a valid path home
  /// @param[in] _val if input is true then there is a valid path, if it is false then there is not a valid path
  //----------------------------------------------------------------------------------------------------------------------
  void setHasPathHome(const bool _val) { m_hasPathHome = _val; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the health of the agent
  /// @return float value of agent health
  //----------------------------------------------------------------------------------------------------------------------
  float getHealth() const { return m_health; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the health of the agent
  /// @param[in] _val input to set the health to
  //----------------------------------------------------------------------------------------------------------------------
  void setHealth(const float _val) { m_health = _val; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the energy of the agent
  /// @return float value of agent energy
  //----------------------------------------------------------------------------------------------------------------------
  float getEnergy() const { return m_energy; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the energy of the agent
  /// @param[in] _val input to set the energy to
  //----------------------------------------------------------------------------------------------------------------------
  void setEnergy(const float _val) { m_energy = _val; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the morale of the agent
  /// @return float value of agent morale
  //----------------------------------------------------------------------------------------------------------------------
  float getMorale() const { return m_morale; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the morale of the agent
  /// @param[in] _val input to set the morale to
  //----------------------------------------------------------------------------------------------------------------------
  void setMorale(const float _val) { m_morale = _val; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the rage of the agent
  /// @return float value of agent rage
  //----------------------------------------------------------------------------------------------------------------------
  float getRage() const { return m_rage; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the rage of the agent
  /// @param[in] _val input to set the rage to
  //----------------------------------------------------------------------------------------------------------------------
  void setRage(const float _val) { m_rage = _val; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the damage of the agent
  /// @return float value of agent damage
  //----------------------------------------------------------------------------------------------------------------------
  float getDamage() const { return m_damage; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the damage of the agent
  /// @param[in] _val input to set the damage to
  //----------------------------------------------------------------------------------------------------------------------
  void setDamage(const float _val) { m_damage = _val; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the maximum speed of the agent
  /// @return float value of the maximum speed of the agent
  //----------------------------------------------------------------------------------------------------------------------
  float getMaxSpeed()const{return m_maxSpeed;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the maximum speed of the agent
  /// @param[in] _maxSpeed input to set the maximum speed of the agent
  //----------------------------------------------------------------------------------------------------------------------
  void setMaxSpeed(const float _maxSpeed){m_maxSpeed = _maxSpeed;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the height of the agent's hop for drawing
  /// @return float value of how high the agent's hop is drawn
  //----------------------------------------------------------------------------------------------------------------------
  float getHopHeight() const { return m_hopHeight; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the height of the agent's hop for drawing
  /// @param[in] _val input to set how height the agent hops
  //----------------------------------------------------------------------------------------------------------------------
  void setHopHeight(const float _val) { m_hopHeight = _val; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the speed of the agent's hop
  /// @return float value of how frequently the agent hops
  //----------------------------------------------------------------------------------------------------------------------
  float getHopSpeed() const { return m_hopSpeed; }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief sets the speed of the agent's hop
  /// @param[in] _val input to set how freqently the agent hops
  //----------------------------------------------------------------------------------------------------------------------
  void setHopSpeed(float _val) { m_hopSpeed = _val; }

protected:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the Lua state registered with the agent
  //----------------------------------------------------------------------------------------------------------------------
  lua_State *L;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief a pointer to the Timer monostate class
  //----------------------------------------------------------------------------------------------------------------------
  Timer* m_gameTimer;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the mesh used to draw the agent in the world
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Obj *m_mesh;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the value of the target agent's ID
  //----------------------------------------------------------------------------------------------------------------------
  int m_targetID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the position vector of the agent's home
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_homePos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief if the agent has a valid path home
  //----------------------------------------------------------------------------------------------------------------------
  bool m_hasPathHome;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the detection radius of the agent
  //----------------------------------------------------------------------------------------------------------------------
  float m_detectionRadius;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the health value of the agent
  //----------------------------------------------------------------------------------------------------------------------
  float m_health;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the energy value of the agent
  //----------------------------------------------------------------------------------------------------------------------
  float m_energy;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the morale value of the agent
  //----------------------------------------------------------------------------------------------------------------------
  float m_morale;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the rage value of the agent
  //----------------------------------------------------------------------------------------------------------------------
  float m_rage;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the damage value of the agent
  //----------------------------------------------------------------------------------------------------------------------
  float m_damage;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the current value of the agent's height in it's hop sine cycle
  //----------------------------------------------------------------------------------------------------------------------
  double m_hop;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the hop height value of the agent
  //----------------------------------------------------------------------------------------------------------------------
  float m_hopHeight;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief the hop frequency of the agent
  //----------------------------------------------------------------------------------------------------------------------
  float m_hopSpeed;
};

#endif // AGENT_H
