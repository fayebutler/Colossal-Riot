#ifndef STATEMACHINE_H
#define STATEMACHINE_H

//----------------------------------------------------------------------------------------------------------------------------
/// @file StateMachine.h
/// @class StateMachine "include/StateMachine.h"
/// @brief Templated scripted FSM to make agent decisions linked using LuaBridge
/// @author Emily Holt
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

#include "assert.h"
#include <typeinfo>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

// Using LuaBridge found at https://github.com/vinniefalco/LuaBridge
#include "LuaBridge.h"


template <class entityType> class StateMachine
{
public:
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief StateMachine ctor gets the lua state from its owner to register its functions
  /// @param[in] _owner: Pointer to the owner(agent) of the state machine
  //----------------------------------------------------------------------------------------------------------------------------
  StateMachine(entityType* _owner):m_owner(_owner)
  {
    L = m_owner->getLuaState();
    registerLua(L);
    luabridge::push(L, this);
    lua_setglobal(L, "stateMachine");
    m_currentState = NULL;
    m_previousState = NULL;
  }

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief State Machine dtor
  //----------------------------------------------------------------------------------------------------------------------------
  ~StateMachine() {}

  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the current state (of its owner)
  /// @param[in] _instate: pass in the state to set current state to
  //----------------------------------------------------------------------------------------------------------------------------
  void setCurrentState(const char* _inState) { m_currentState = _inState; }
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the current state
  /// @return const char*: the current state
  //----------------------------------------------------------------------------------------------------------------------------
  const char* getCurrentState() const { return m_currentState; }
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the previous state
  /// @param[in] _inState: pass in the state to set previous state to
  //----------------------------------------------------------------------------------------------------------------------------
  void setPreviousState(const char* _inState) { m_previousState = _inState; }
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the current state
  /// @return const char*: the current state
  //----------------------------------------------------------------------------------------------------------------------------
  const char* getPreviousState() const { return m_previousState; }
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the global state
  /// @param[in] _inState: pass in the state to set global state to
  //----------------------------------------------------------------------------------------------------------------------------
  void setGlobalState(const char* _inState) { m_globalState = _inState; }
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Gets the current global state
  /// @return const char*: the current global state
  //----------------------------------------------------------------------------------------------------------------------------
  const char* getGlobalState() const { return m_globalState; }

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief update function that checks the global state for changes and then the execute funciton of the current state
  //----------------------------------------------------------------------------------------------------------------------
  void update()
  {
    //check global state in lua for variable or state changes that apply to every state
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

    //call execute function of current state to check for variable or state changes
    else if (m_currentState)
    {
      luaCallState(m_currentState, "execute");
    }

    //check agent properties against limits set in lua i.e. health not rising above 100
    luaCallState("limits", "check");
  }

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief function to lookup and call a lua function in a specified table from c++
  /// @param[in] _state: the state table in lua to lookup
  /// @param[in] _phase: the 'phase' of the state reflecting a function in the lua table ie. exit
  //----------------------------------------------------------------------------------------------------------------------
  void luaCallState(const char* _state, const char* _phase)
  {
    luabridge::LuaRef state = luabridge::getGlobal(L, _state);
    state[_phase]();
  }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief function to change state by calling the exit function of the current state and then the enter function of the new state
  /// @param[in] _newState: the new state table to lookup in lua
  //----------------------------------------------------------------------------------------------------------------------
  void changeState(const char* _newState)
  {
    m_previousState = m_currentState;

    //call exit function of current state
    if (m_currentState != NULL)
    {
      m_previousState = m_currentState;
      luaCallState(m_currentState, "exit");
    }
    //call enter function of new state
    m_currentState = _newState;
    luaCallState(m_currentState, "enter");
  }

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief function to register the state machine class using LuaBridge
  /// @param[in] _L: the lua state to register the class to
  //----------------------------------------------------------------------------------------------------------------------
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
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the state machine owner
  //----------------------------------------------------------------------------------------------------------------------------
  entityType* m_owner;
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the current state
  //----------------------------------------------------------------------------------------------------------------------------
  const char* m_currentState;
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the previous state
  //----------------------------------------------------------------------------------------------------------------------------
  const char* m_previousState;
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the global state
  //----------------------------------------------------------------------------------------------------------------------------
  const char* m_globalState;
  //----------------------------------------------------------------------------------------------------------------------------
  /// @brief Pointer to the lua state passed in by the owner
  //----------------------------------------------------------------------------------------------------------------------------
  lua_State *L;
};

#endif // STATEMACHINE_H
