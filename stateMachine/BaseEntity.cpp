#include "BaseEntity.h"

int BaseEntity::m_nextValidID = 0;

BaseEntity::BaseEntity()
{
    L = luaL_newstate();

    setID(m_nextValidID);
    m_nextValidID++;

   EntityMgr->registerEntity(this);
}

BaseEntity::~BaseEntity()
{

}

void BaseEntity::update()
{

}

bool BaseEntity::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {
  case msgAttack:
    m_health -= _message.m_extraInfo;
    return true;

  default:
    std::cout<<"Message type not defined"<<std::endl;
    return false;
  }
}

void BaseEntity::registerLua(lua_State* _L)
{
    luabridge::getGlobalNamespace(_L)
        .beginClass<BaseEntity>("BaseEntity")
                .addFunction("getID", &BaseEntity::getID)
                .addProperty("m_morale", &BaseEntity::getMorale, &BaseEntity::setMorale)
                .addProperty("m_health", &BaseEntity::getHealth, &BaseEntity::setHealth)
                .addProperty("m_rage", &BaseEntity::getRage, &BaseEntity::setRage)
                .addProperty("m_targetID", &BaseEntity::getTargetID, &BaseEntity::setTargetID)

        .endClass();
}
