#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "assert.h"
#include <typeinfo>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "LuaBridge.h"

template <class entityType> class StateMachine
{
public:
    StateMachine(entityType* _owner):m_owner(_owner)
    {
        L = m_owner->getLuaState();
        registerLua(L);
        luabridge::push(L, this);
        lua_setglobal(L, "stateMachine");
        m_currentState = NULL;
        m_previousState = NULL;

    }

    ~StateMachine() {}

    void setCurrentState(const char* _inState) { m_currentState = _inState; }
    const char* getCurrentState() const { return m_currentState; }

    void setPreviousState(const char* _inState) { m_previousState = _inState; }
    const char* getPreviousState() const { return m_previousState; }

    void setGlobalState(const char* _inState) { m_globalState = _inState; }
    const char* getGlobalState() const { return m_globalState; }

    void update()
    {
      if (m_globalState)
      {
        luaCallState(m_globalState, "execute");
      }

      //call enter on first tick to set state behaviours
      if(!m_previousState && m_currentState)
      {
          luaCallState(m_currentState, "enter");
          m_previousState = m_currentState;
      }

      else if (m_currentState)
      {
        luaCallState(m_currentState, "execute");
      }

    }

    void luaCallState(const char* _state, const char* _phase)
    {
        luabridge::LuaRef state = luabridge::getGlobal(L, _state);
        state[_phase]();
    }

    void changeState(const char* _newState)
    {

        m_previousState = m_currentState;

        if (m_currentState != NULL)
        {
            m_previousState = m_currentState;
            luaCallState(m_currentState, "exit");
        }

        m_currentState = _newState;
        luaCallState(m_currentState, "enter");
    }


    void registerLua(lua_State* _L)
    {
        luabridge::getGlobalNamespace(_L)
            .beginClass<StateMachine>("StateMachine")
                    .addFunction("changeState", &StateMachine::changeState)
                    .addProperty("m_currentState", &StateMachine::getCurrentState, &StateMachine::setCurrentState)
                    .addProperty("m_previousState", &StateMachine::getPreviousState, &StateMachine::setPreviousState)
                    .addProperty("m_globalState", &StateMachine::getGlobalState, &StateMachine::setGlobalState)

            .endClass();
    }


private:
    entityType* m_owner;
    const char* m_currentState;
    const char* m_previousState;
    const char* m_globalState;


    lua_State *L;
};

#endif // STATEMACHINE_H
