#include "Agent.h"

Agent::Agent(GameWorld* world)
{
  m_vehicle = new Vehicle(world, ngl::Vec3(0,0,0), ngl::Vec3(0,0,0), 0.0f, 1.0f, 1.0f,1.0f, 1.0f, 0.5f);
  m_ID = m_vehicle->getID();
  L = luaL_newstate();
}

Agent::~Agent()
{

}

void Agent::update(double timeElapsed)
{
  m_vehicle->update(timeElapsed);
}

void Agent::registerLua(lua_State* _L)
{
    luabridge::getGlobalNamespace(_L)
        .beginClass<Agent>("Agent")
                .addFunction("getID", &Agent::getID)
                .addProperty("m_morale", &Agent::getMorale, &Agent::setMorale)
                .addProperty("m_health", &Agent::getHealth, &Agent::setHealth)
                .addProperty("m_rage", &Agent::getRage, &Agent::setRage)
                .addProperty("m_targetID", &Agent::getTargetID, &Agent::setTargetID)

        .endClass();
}
