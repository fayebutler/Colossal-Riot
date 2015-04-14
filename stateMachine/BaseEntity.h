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

//HIIII

class BaseEntity
{
public:
  BaseEntity();
  virtual ~BaseEntity();

  virtual void update() = 0;

  int getID() const { return m_ID; }

  virtual bool handleMessage(const Message& _message) = 0;

  void registerLua(lua_State *_L);

protected:
  int m_ID;
  static int m_nextValidID;
  void setID(int _val) { m_ID = _val; }

  lua_State *L;

};

#endif // BASEENTITY_H
