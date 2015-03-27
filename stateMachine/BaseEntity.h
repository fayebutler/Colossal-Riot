#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <iostream>
#include "EntityManager.h"
#include "Message.h"
#include "MessageManager.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "LuaBridge.h"

class BaseEntity
{
public:
  BaseEntity();
  virtual ~BaseEntity();

  virtual void update() = 0;

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

  virtual bool handleMessage(const Message& _message) = 0;

  void registerLua(lua_State *_L);

protected:
  int m_ID;
  static int m_nextValidID;
  void setID(int _val) { m_ID = _val; }

  float m_health;
  float m_energy;
  float m_morale;
  float m_rage;

  int m_targetID;

  lua_State *L;

};

#endif // BASEENTITY_H
