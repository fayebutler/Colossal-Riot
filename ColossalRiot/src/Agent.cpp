#include "Agent.h"

Agent::Agent(GameWorld* world): Vehicle(world, ngl::Vec3(0,0,0), ngl::Vec3(0,0,0), 0.0f, 1.0f, 10.0f,1.0f, 1.0f, 0.5f)
{

  L = luaL_newstate();

  m_targetID = -1;

}

Agent::~Agent()
{

}

void Agent::update(double timeElapsed, double currentTime)
{
  Vehicle::update(timeElapsed);
}

void Agent::setTargetID(int _val)
{
    m_targetID = _val;
    if (_val < 0)
    {
//        std::cout<<"TARGETID NULL : "<<m_targetID<<std::endl;
        Vehicle::Steering()->setTargetAgent(NULL);
    }
    else
    {
      Vehicle::Steering()->setTargetAgent((Vehicle*)m_entityMgr->getEntityFromID(m_targetID));
    }
}

void Agent::wander(double weight)
{
    if(weight <= 0.0)
    {
      Vehicle::Steering()->WanderOff();
    }
    else
    {
      Vehicle::Steering()->WanderOn();
      Vehicle::Steering()->setWanderWeight(weight);
    }
}

void Agent::pursuit(double weight)
{
    if(weight <= 0.0)
    {
      Vehicle::Steering()->PursuitOff();
    }
    else
    {        
      Vehicle::Steering()->PursuitOn();
      Vehicle::Steering()->setPursuitWeight(weight);
    }
}

void Agent::evade(double weight)
{
    if(weight <= 0.0)
    {
      Vehicle::Steering()->EvadeOff();
    }
    else
    {
      Vehicle::Steering()->EvadeOn();
      Vehicle::Steering()->setEvadeWeight(weight);
    }
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

void Agent::checkValidTarget(float _dist, float _health)
{
    if(m_targetID >= 0)
    {
        Agent* target = dynamic_cast<Agent*>(m_entityMgr->getEntityFromID(m_targetID));

        ngl::Vec3 toEntity = m_pos - target->getPos();
        double distSqFromEachOther = toEntity.lengthSquared();

        // change target
        if(distSqFromEachOther > ((m_boundingRadius * _dist) * (m_boundingRadius * _dist)) || target->getHealth() < _health)
        {
            findTargetID(_health);
        }
        else
        {
//            std::cout<<"TARGET A-OKAY"<<std::endl;
        }
    }
    else
    {
//        std::cout<<"TARGET NOT SET"<<std::endl;
        findTargetID(_health);
    }
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
                .addFunction("getTargetID", &Agent::getTargetID)
                .addFunction("setTargetID", &Agent::setTargetID)
                .addProperty("m_targetID", &Agent::getTargetID, &Agent::setTargetID)
                .addFunction("wander", &Agent::wander)
                .addFunction("pursuit", &Agent::pursuit)
                .addFunction("evade", &Agent::evade)
                .addFunction("checkValidTarget", &Agent::checkValidTarget)

        .endClass();
}
