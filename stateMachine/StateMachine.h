#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "assert.h"
#include <typeinfo>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

template <class entityType> class StateMachine
{
public:
    StateMachine(entityType* _owner):m_owner(_owner)
    {
        L = m_owner->getLuaState();
    }

    void setCurrentState(const char* _inState) { m_currentState = _inState; }
    const char* getCurrenState() const { return m_currentState; }
    void setPreviousState(const char* _inState) { m_previousState = _inState; }
    const char* getPreviousState() const { return m_previousState; }
    void setGlobalState(const char* _inState) { m_globalState = _inState; }
    const char* getGlobalState() const { return m_globalState; }

    void update()    {
//      if (m_globalState)
//      {
//        m_globalState->execute(m_owner);
//      }
      if (m_currentState)
      {
        luaCallState(m_currentState);
      }
    }

    ~StateMachine() {}

    void luaCallState(const char* _state)
    {
      lua_settop(L, 0);
      lua_getglobal(L, _state);
      if (!lua_istable(L, -1))
      {
        std::cout<<"[C++] ERROR: Lua table '"<<_state<<"' not defined"<<std::endl;
      }
      else
      {
        lua_pushstring(L, "execute");
        lua_gettable(L, -2);
        if (!lua_isfunction(L, -1))
        {
          std::cout<<"[C++] ERROR: Lua function 'execute' not defined"<<std::endl;
        }
        else
        {
          lua_pushstring(L, m_currentState);
          lua_pushstring(L, m_previousState);
          lua_pushnumber(L, m_owner->getMorale());
          lua_call(L, 3, 3);
          m_currentState = lua_tostring(L, -3);
          std::cout<<"m_currentState = "<<m_currentState<<std::endl;
          m_previousState = lua_tostring(L, -2);
          //std::cout<<"m_previousState = "<<m_previousState<<std::endl;
          m_owner->setMorale(lua_tonumber(L, -1));
          std::cout<<"m_morale = "<<m_owner->getMorale()<<std::endl;
          lua_settop(L,0);
        }
      }
    }


private:
    entityType* m_owner;
    const char* m_currentState;
    const char* m_previousState;
    const char* m_globalState;

    lua_State *L;
};

#endif // STATEMACHINE_H
