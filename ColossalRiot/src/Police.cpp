#include "Police.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

Police::Police(GameWorld* world) : Agent(world)
{
    // Set up LUA state
    luaL_dofile(L, "lua/Police.lua");
    luaL_openlibs(L);

    registerClass(L);
    luabridge::push(L, this);
    lua_setglobal(L, "police");

    // Set up state machine
    m_stateMachine = new StateMachine<Police>(this);
//    m_stateMachine->setGlobalState(Class::Instance());

    // Set initial variables
    m_targetID = 0;
    luabridge::LuaRef makePolice = luabridge::getGlobal(L, "makePolice");
    makePolice();

}

Police::~Police()
{
  lua_close(L);
  delete m_stateMachine;
}

void Police::update(double timeElapsed)
{
    Agent::update(timeElapsed);
    m_stateMachine->update();

}



//bool Police::handleMessage(const Message& _message)
//{
//  Agent::handleMessage(_message);
//}


//POLICE STATE UTILITY FUNCTIONS

void Police::attack(int _ID)
{
  std::cout<<"PO PO Attack: "<<_ID<<" for "<<m_rage<<std::endl;
  MessageMgr->sendMessage(this->getID(),m_targetID,msgAttack,0,m_rage);
}

void Police::registerClass(lua_State* _L)
{
    registerLua(_L);
    luabridge::getGlobalNamespace(_L)
        .deriveClass<Police, Agent>("Police")
            .addConstructor <void (*) (GameWorld*)> ()
                .addFunction("attack", &Police::attack)
        .endClass();
}
