#include "Agent.h"
Agent::Agent(GameWorld* world): Vehicle(world, ngl::Vec3(0,0,0), ngl::Vec3(0,0,0), 0.0f, 1.0f, 10.0f,1.0f, 1.0f, 0.5f)
{

  L = luaL_newstate();

  m_targetID = -1;

  m_hasPathHome = false;

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

void Agent::seek(double weight)
{
    if(weight <= 0.0)
    {
      Vehicle::Steering()->SeekOff();
    }
    else
    {
      Vehicle::Steering()->SeekOn();
      Vehicle::Steering()->setSeekWeight(weight);
    }
}

void Agent::arrive(double weight)
{
    if(weight <= 0.0)
    {
      Vehicle::Steering()->ArriveOff();
    }
    else
    {
      Vehicle::Steering()->ArriveOn();
      Vehicle::Steering()->setArriveWeight(weight);
    }
}

void Agent::cohesion(double weight)
{
    if(weight <= 0.0)
    {
      Vehicle::Steering()->CohesionOff();
    }
    else
    {
      Vehicle::Steering()->CohesionOn();
      Vehicle::Steering()->setCohesionWeight(weight);
    }
}

void Agent::separation(double weight)
{
    if(weight <= 0.0)
    {
      Vehicle::Steering()->SeparationOff();
    }
    else
    {
      Vehicle::Steering()->SeparationOn();
      Vehicle::Steering()->setSeparationWeight(weight);
    }
}

void Agent::alignment(double weight)
{
    if(weight <= 0.0)
    {
      Vehicle::Steering()->AlignmentOff();
    }
    else
    {
      Vehicle::Steering()->AlignmentOn();
      Vehicle::Steering()->setAlignmentWeight(weight);
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
      std::map<int, BaseGameEntity*>::const_iterator entity = m_entityMgr->getEntityMap().find(m_targetID);

      if(entity->first !=  m_entityMgr->getEntityMap().end()->first)
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
        findTargetID(_health);
      }
    }
    else
    {
//        std::cout<<"TARGET NOT SET"<<std::endl;
        findTargetID(_health);
    }

}

bool Agent::targetWithinReach(float _reach)
{
    Agent* target = dynamic_cast<Agent*>(m_entityMgr->getEntityFromID(m_targetID));
    if((m_pos - target->getPos()).lengthSquared() <= _reach)
    {
        return true;
    }
    else
    {
        return false;
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
            .addFunction("wander", &Agent::wander)
            .addFunction("pursuit", &Agent::pursuit)
            .addFunction("evade", &Agent::evade)
            .addFunction("arrive", &Agent::arrive)
            .addFunction("seek", &Agent::seek)
            .addFunction("cohesion", &Agent::cohesion)
            .addFunction("separation", &Agent::separation)
            .addFunction("alignment", &Agent::alignment)
            .addFunction("checkValidTarget", &Agent::checkValidTarget)
            .addFunction("targetWithinReach", &Agent::targetWithinReach)
        .endClass();
}
