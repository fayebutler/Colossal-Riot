#include "Police.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

Police::Police() : BaseEntity()
{
    // Set up LUA state
    L = luaL_newstate();
    luaL_openlibs(L);
    luaL_loadfile(L,"PoliceStates.lua");
    lua_call(L, 0, 0);

    // Set initial variables
    m_morale = 100;

    // Set up state machine
    m_stateMachine = new StateMachine<Police>(this);
    m_stateMachine->setCurrentState("work");
    m_stateMachine->setPreviousState("NULL");
//    m_stateMachine->setGlobalState(Class::Instance());
}

Police::~Police()
{
   lua_close(L);
   delete m_stateMachine;
}

void Police::update()
{
    m_stateMachine->update();
}

bool Police::handleMessage(const Message& _message)
{
    BaseEntity::handleMessage(_message);
}
