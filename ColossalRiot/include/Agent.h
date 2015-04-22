#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include "Vehicle.h"

#include <ngl/ShaderLib.h>
#include <ngl/NGLInit.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "LuaBridge.h"

class Agent: public Vehicle
{
public:
  Agent(GameWorld *world);
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
  void setTargetID(int _val) { m_targetID = _val; }

  float getHopHeight() const { return m_hopHeight; }
  void setHopHeight(float _val) { m_hopHeight = _val; }

  float getHopSpeed() const { return m_hopSpeed; }
  void setHopSpeed(float _val) { m_hopSpeed = _val; }

  void registerLua(lua_State *_L);

  virtual bool handleMessage(const Message& _message);

  //STEERING FUNCTIONS

  void wander(double weight);

  void pursuit(double weight);

  void evade(double weight);


protected:
  float m_health;
  float m_energy;
  float m_morale;
  float m_rage;

  float m_damage;

  int m_targetID;

  double m_hop;
  float m_hopHeight;
  float m_hopSpeed;

  lua_State *L;

};

#endif // AGENT_H
