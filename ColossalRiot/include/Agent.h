#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include "Vehicle.h"
//#include "EntityManager.h"

#include <ngl/ShaderLib.h>
#include <ngl/Material.h>
#include <ngl/NGLInit.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Obj.h>
#include <Timer.h>


extern "C" {
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

  virtual void update(double timeElapsed, double currentTime) = 0;

  float getHealth() const { return m_health; }
  void setHealth(float _val) { m_health = _val; }

  float getEnergy() const { return m_energy; }
  void setEnergy(float _val) { m_energy = _val; }

  float getMorale() const { return m_morale; }
  void setMorale(float _val) { m_morale = _val; }

  float getRage() const { return m_rage; }
  void setRage(float _val) { m_rage = _val; }

  float getDamage() const { return m_damage; }
  void setDamage(float _val) { m_damage = _val; }

  int getTargetID() const { return m_targetID; }

  void setTargetID(int _val);

  virtual void findTargetID(float _health) = 0;

  void checkValidTarget(float _dist, float _health, float _rage);

  ngl::Vec3 getHomePos() const { return m_homePos; }
  void setHomePos(ngl::Vec3 _val) { m_homePos = _val; }

  bool getHasPathHome() const { return m_hasPathHome; }
  void setHasPathHome(bool _val) { m_hasPathHome = _val; }

  float getHopHeight() const { return m_hopHeight; }
  void setHopHeight(float _val) { m_hopHeight = _val; }

  float getHopSpeed() const { return m_hopSpeed; }
  void setHopSpeed(float _val) { m_hopSpeed = _val; }

  float getMaxSpeed()const{return m_maxSpeed;}
  void setMaxSpeed(float max_speed){m_maxSpeed = max_speed;}

  void registerLua(lua_State *_L);

  virtual bool handleMessage(const Message& _message);

  bool targetWithinReach(float _reach);

  //STEERING FUNCTIONS

  void wander(double weight);

  void pursuit(double weight);

  void evade(double weight);

  void seek(double weight);
  void arrive(double weight);

  void cohesion(double weight);

  void separation(double weight);

  void alignment(double weight);


protected:

  float m_detectionRadius;

  float m_health;
  float m_energy;
  float m_morale;
  float m_rage;

  ngl::Obj *m_mesh;
  Timer* m_gameTimer;


  int m_targetID;

  float m_damage;

  double m_hop;
  float m_hopHeight;
  float m_hopSpeed;

  ngl::Vec3 m_homePos;
  bool m_hasPathHome;

  lua_State *L;

};

#endif // AGENT_H
