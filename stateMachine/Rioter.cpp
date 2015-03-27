#include "Rioter.h"

Rioter::Rioter() : BaseEntity()
{
    // Set up LUA state
    luaL_dofile(L, "Rioter.lua");
    luaL_openlibs(L);

    registerClass(L);
    luabridge::push(L, this);
    lua_setglobal(L, "rioter");

    // Set up state machine
    m_stateMachine = new StateMachine<Rioter>(this);
//    m_stateMachine->setGlobalState(Class::Instance());

    // Set initial variables
    m_targetID = 1;
    luabridge::LuaRef makeRioter = luabridge::getGlobal(L, "makeRioter");
    makeRioter();

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
  std::cout<<"RIOT RIOT Attack: "<<_ID<<" for "<<m_rage<<std::endl;
  MessageMgr->sendMessage(this->getID(),m_targetID,msgAttack,0,m_rage);
}

void Rioter::registerClass(lua_State* _L)
{
    registerLua(_L);
    luabridge::getGlobalNamespace(_L)
        .deriveClass<Rioter, BaseEntity>("Rioter")
            .addConstructor <void (*) (void)> ()
                .addFunction("attack", &Rioter::attack)
        .endClass();
}
