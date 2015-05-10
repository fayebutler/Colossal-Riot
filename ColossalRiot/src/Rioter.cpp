//----------------------------------------------------------------------------------------------------------------------------
/// @file Rioter.cpp
/// @brief defines a rioter entity
//----------------------------------------------------------------------------------------------------------------------------

#include "Rioter.h"
#include "GameWorld.h"

//----------------------------------------------------------------------------------------------------------------------------
Rioter::Rioter(GameWorld* world, ngl::Obj *_mesh) : Agent(world)
{
    m_entityType = typeRioter;
    //m_gameworld = world;

    // Set up LUA state
    luaL_dofile(L, "lua/Rioter.lua");
    luaL_openlibs(L);

  // Set up LUA state
  luaL_dofile(L, "lua/Rioter.lua");
  luaL_openlibs(L);

  registerClass(L);
  luabridge::push(L, this);
  lua_setglobal(L, "rioter");

  // Set up state machine
  m_stateMachine = new StateMachine<Rioter>(this);

  // Set initial variables
  m_mesh = _mesh;

  m_hopHeight = 0.5;
  m_hopSpeed = 0.0;
  luabridge::LuaRef makeRioter = luabridge::getGlobal(L, "makeRioter");
  makeRioter();

  Vehicle::Steering()->WallAvoidOn();
  Vehicle::Steering()->setWallAvoidWeight(0.4);
  Vehicle::Steering()->ObstacleAvoidOn();
  Vehicle::Steering()->setObstacleAvoidWeight(0.4);

  m_policeInfluence = 0.0;

  m_protestPos = ngl::Vec3(0,0,0);
}

//----------------------------------------------------------------------------------------------------------------------------
Rioter::~Rioter()
{
  lua_close(L);
  delete m_stateMachine;
}

//----------------------------------------------------------------------------------------------------------------------------
void Rioter::update(double timeElapsed, double currentTime)
{
  Vehicle::Steering()->clearFriendlyNeighbours();
  Vehicle::Steering()->clearAllNeighbours();
  Vehicle::Steering()->addFriendlyNeighbours(getNeighbourRioterIDs());
  Vehicle::Steering()->addAllNeighbours(getNeighbourRioterIDs());
  Vehicle::Steering()->addAllNeighbours(getNeighbourPoliceIDs());

  Vehicle::Steering()->WallOverlapAvoidance();
  Vehicle::Steering()->ObjectOverlapAvoidance();

  Agent::update(timeElapsed, currentTime);
  m_stateMachine->update();

  // calculate influence of neighbouring police based on their rage
  int nearbyPolice = m_neighbourPoliceIDs.size();
  m_policeInfluence = 0.0;

  for (int i=0; i<nearbyPolice; i++)
  {
    Agent* policeman = dynamic_cast<Agent*>(m_entityMgr->getEntityFromID(m_neighbourPoliceIDs[i]));
    if (policeman)
    {
      m_policeInfluence += policeman->getRage();
    }
  }

  m_hopSpeed = m_rage/5.0;
  m_hop = (sin((currentTime*m_hopSpeed)+m_ID)*sin((currentTime*m_hopSpeed)+m_ID)*m_hopHeight);
}

//----------------------------------------------------------------------------------------------------------------------------
void Rioter::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{
  loadMatricesToShader(cam, mouseGlobalTX);
  m_mesh->draw();
}

//----------------------------------------------------------------------------------------------------------------------------
void Rioter::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
  m.setSpecularExponent(5.f);
  m.setDiffuse(ngl::Colour(1.0f-(1-(getHealth()/100.0f)), 1.0f-(getRage()/100.0f)-(1-(getHealth()/100.0f)), 1.0f-(getRage()/100.0f)-(1-(getHealth()/100.0f)), 1.0f));

  m.loadToShader("material");

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  ngl::Transformation trans;
  trans.setPosition(getPos().m_x,m_hop,getPos().m_z);

  ngl::Real rot = atan(getHeading().m_z/getHeading().m_x);


  rot = ((rot * 180)/M_PI);

  if(getHeading().m_x < 0)
  {
    rot = 180 + rot;
  }

  if(m_health <= 20.f)
  {
    trans.addPosition(0,0.3,0);
    trans.setRotation(90,-rot+90,0);
  }
  else
  {
    trans.setRotation(0,-rot+90,0);
  }


  M=trans.getMatrix()*mouseGlobalTX;
  MV=  M*cam->getViewMatrix();
  MVP= M*cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);
}

//----------------------------------------------------------------------------------------------------------------------------
void Rioter::findTargetID(float _health)
{
  std::vector<int> police = getNeighbourPoliceIDs();
  float currentHealth = -1;
  Agent* currentTarget = NULL;
  int numberOfPolice = police.size();
  for (int i=0; i<numberOfPolice; i++)
  {
    Agent* policeman = dynamic_cast<Agent*>(m_entityMgr->getEntityFromID(police[i]));
    if (policeman)
    {
      if (policeman->getHealth()> _health && policeman->getHealth()>currentHealth)
      {
        currentHealth = policeman->getHealth();
        currentTarget = policeman;
      }
    }
  }

  if (currentTarget == NULL)
  {
    setTargetID(-1);
  }
  else
  {
    int target = currentTarget->getID();
    setTargetID(target);
  }
}

//----------------------------------------------------------------------------------------------------------------------------
bool Rioter::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {
    case msgRioterDeath:
    {
      m_morale -= 5.f;
      m_rage += 30.f;
      return true;
      break;
    }
    case msgPoliceDeath:
    {
      m_morale -= 15.f;
      return true;
      break;
    }
    case msgAttack:
    {
      return Agent::handleMessage(_message);
      break;
    }
    default:
    {
      std::cout<<"Rioter: Message type not defined"<<std::endl;
      return false;
      break;
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------
void Rioter::attack()
{
  m_messageMgr->sendMessage(this->getID(), this->getTargetID(), msgAttack, m_damage);
}

//----------------------------------------------------------------------------------------------------------------------------
void Rioter::death()
{
  for (int i = 0; i < m_world->getNumberOfRioters(); i++)
  {
    ngl::Vec3 vecToRioter = m_world->getRioters()[i]->getPos() - m_pos;
    double distSqToRioter = vecToRioter.lengthSquared();
    double affectedRadius = 8.0;
    if (distSqToRioter < affectedRadius * affectedRadius)
    {
      m_messageMgr->sendMessage(this->getID(), m_world->getRioters()[i]->getID(), msgRioterDeath, 0.f);
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------
void Rioter::protestCohesion(double weight)
{
  if(weight <= 0.0)
  {
    Vehicle::Steering()->SquadCohesionOff();
  }
  else
  {
    Vehicle::setSquadCrosshair(m_protestPos);
    Vehicle::Steering()->setSquadCohesionWeight(weight);
    Vehicle::Steering()->SquadCohesionOn();
  }
}

//----------------------------------------------------------------------------------------------------------------------------
void Rioter::findPathHome()
{
  m_homePos = findNearestExit(m_world->getCellGraph()->getExitPoints());
  findPath(m_homePos);
  m_hasPathHome = true;
}

//----------------------------------------------------------------------------------------------------------------------------
void Rioter::registerClass(lua_State* _L)
{
  registerLua(_L);
  luabridge::getGlobalNamespace(_L)
    .deriveClass<Rioter, Agent>("Rioter")
      .addConstructor <void (*) (GameWorld*, ngl::Obj*)> ()
        .addFunction("attack", &Rioter::attack)
        .addFunction("death", &Rioter::death)
        .addFunction("protestCohesion", &Rioter::protestCohesion)
        .addFunction("getPoliceInfluence", &Rioter::getPoliceInfluence)
        .addFunction("findPathHome", &Rioter::findPathHome)
    .endClass();
}
