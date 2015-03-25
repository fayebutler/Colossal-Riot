#include "Rioter.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

Rioter::Rioter() : BaseEntity()
{
    // Set up LUA state
    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_loadfile(L,"RioterStates.lua");

    lua_register(L, "cpp_attack", cpp_attack);

    lua_call(L, 0, 0);

    // Set initial variables
    m_morale = 100;
    m_targetID = 1;

    // Set up state machine
    m_stateMachine = new StateMachine<Rioter>(this);
    m_stateMachine->setCurrentState("protest");
    m_stateMachine->setPreviousState("NULL");
//    m_stateMachine->setGlobalState(Class::Instance());

}

Rioter::~Rioter()
{
  lua_close(L);
  delete m_stateMachine; 
}

void Rioter::update()
{
    m_stateMachine->update();

}

bool Rioter::handleMessage(const Message& _message)
{
  BaseEntity::handleMessage(_message);
}


//RIOTER STATE UTILITY FUNCTIONS

void Rioter::attack(int _ID)
{
  std::cout<<"ATTACK ATTACK ATTACK: "<<_ID<<std::endl;
  //MessageMgr->sendMessage(this->getID(),m_targetID,msgMoraleDown,0,m_rage);
}

int Rioter::cpp_attack(lua_State* L)
{
  int n = lua_gettop(L);
  if (n!=1)
  {
    std::cout<<"Wrong number of arguments"<<std::endl;
    return 0;
  }
  if(!lua_isnumber(L,1))
  {
    std::cout<<"Argument is the wrong type"<<std::endl;
  }

  int _ID = (int)lua_tonumber(L, 1);
  attack(_ID);
  return 0;
}
