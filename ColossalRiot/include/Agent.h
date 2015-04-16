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

class Agent
{
public:
  Agent(GameWorld *world);
  virtual ~Agent();

  virtual void update(double timeElapsed) = 0;

  int getID() const { return m_ID; }

  float getHealth() const { return m_health; }
  void setHealth(float _val) { m_health = _val; }

  float getEnergy() const { return m_energy; }
  void setEnergy(float _val) { m_energy = _val; }

  float getMorale() const { return m_morale; }
  void setMorale(float _val) { m_morale = _val; }

  float getRage() const { return m_rage; }
  void setRage(float _val) { m_rage = _val; }

  int getTargetID() const { return m_targetID; }
  void setTargetID(int _val) { m_targetID = _val; }

  void registerLua(lua_State *_L);

  Vehicle* getVehicle() const { return m_vehicle; }

protected:
  int m_ID;

  float m_health;
  float m_energy;
  float m_morale;
  float m_rage;

  int m_targetID;

  lua_State *L;

  Vehicle* m_vehicle;


};

#endif // AGENT_H
