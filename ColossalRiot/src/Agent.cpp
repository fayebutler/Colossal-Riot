#include "Agent.h"

Agent::Agent(GameWorld* world): Vehicle(world, ngl::Vec3(0,0,0), ngl::Vec3(0,0,0), 0.0f, 1.0f, 1.0f,1.0f, 1.0f, 0.5f)
{

  L = luaL_newstate();

}

Agent::~Agent()
{

}

void Agent::update(double timeElapsed)
{
  Vehicle::update(timeElapsed);
}

void Agent::registerLua(lua_State* _L)
{
    luabridge::getGlobalNamespace(_L)
        .beginClass<Agent>("Agent")
                .addFunction("getID", &Agent::getID)
                .addProperty("m_morale", &Agent::getMorale, &Agent::setMorale)
                .addProperty("m_health", &Agent::getHealth, &Agent::setHealth)
                .addProperty("m_rage", &Agent::getRage, &Agent::setRage)
                .addProperty("m_damage", &Agent::getDamage, &Agent::setDamage)
                .addProperty("m_targetID", &Agent::getTargetID, &Agent::setTargetID)

        .endClass();
}

bool Agent::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {
  case msgAttack:
    m_health -= _message.m_extraInfo;
    return true;

  default:
    std::cout<<"Agent: Message type not defined"<<std::endl;
    return false;
  }
}
