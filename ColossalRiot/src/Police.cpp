#include "Police.h"
#include "GameWorld.h"
#include <math.h>
#include "GameWorld.h"

Police::Police(GameWorld* world, ngl::Obj *_mesh) : Agent(world)
{
  m_messageMgr = new MessageManager();

    m_entityType = typePolice;

    registerClass(L);

    // Set up state machine
    m_stateMachine = new StateMachine<Police>(this);

    // Set initial variables
    m_mesh = _mesh;

    m_isMoving = false;
    m_validPursuit = true;

    m_hopHeight = 0.5;
    m_hopSpeed = 0.0;
    luabridge::LuaRef makePolice = luabridge::getGlobal(L, "makePolice");
    makePolice();

    m_blockadePosition = NULL;
    Vehicle::Steering()->WallAvoidOn();
    Vehicle::Steering()->setWallAvoidWeight(0.4);
    Vehicle::Steering()->ObstacleAvoidOn();
//    Vehicle::Steering()->setObstacleAvoidWeight(1.0);

    m_rioterInfluence = 0.0;

   Vehicle::setMaxSpeed(3);


}

Police::~Police()
{
  lua_close(L);
  delete m_stateMachine;
  delete m_messageMgr;
}

void Police::update(double timeElapsed, double currentTime)
{
  //make a friendly and opposing neightbours vector as seperation wants all neighbours bu alignment & cohesion doesn't
  Vehicle::Steering()->clearFriendlyNeighbours();
  Vehicle::Steering()->clearAllNeighbours();
  Vehicle::Steering()->addFriendlyNeighbours(getNeighbourPoliceIDs());
  Vehicle::Steering()->addAllNeighbours(getNeighbourRioterIDs());
  Vehicle::Steering()->addAllNeighbours(getNeighbourPoliceIDs());

  Vehicle::Steering()->WallOverlapAvoidance();
  Vehicle::Steering()->ObjectOverlapAvoidance();

  Agent::update(timeElapsed, currentTime);
  m_stateMachine->update();

  // calculate influence of neighbouring rioters based on their rage
  int nearbyRioters = m_neighbourRioterIDs.size();
  m_rioterInfluence = 0.0;


  for (int i=0; i<nearbyRioters; i++)
  {
      Agent* rioter = dynamic_cast<Agent*>(m_entityMgr->getEntityFromID(m_neighbourRioterIDs[i]));
      if (rioter)
      {
          m_rioterInfluence += rioter->getRage();
      }
  }

  m_hopSpeed = 10.f;
  m_hop = (sin((currentTime*m_hopSpeed)+m_ID)*sin((currentTime*m_hopSpeed)+m_ID)*m_hopHeight);


    if(m_blockadePosition != NULL)
    {
        this->setCrosshair(m_blockadePosition);
//        this->setMass(10.0);
        if((this->getPos() - m_blockadePosition).lengthSquared() <= 0.01)
        {
            this->Steering()->ArriveOff();
//            this->setVelocity(ngl::Vec3(0,0,0));
//            this->setHeading(ngl::Vec3(0,0,1));
//            this->setMaxTurnRate(0.0);
        }
        else
        {
            this->Steering()->ArriveOn();
        }
    }
}


void Police::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{
  loadMatricesToShader(cam, mouseGlobalTX);
  //ngl::VAOPrimitives::instance()->draw("cube");
  m_mesh->draw();
}

void Police::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
      (*shader)["Phong"]->use();

  ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
  m.setSpecularExponent(5.f);
  m.setDiffuse(ngl::Colour(0.0f, 0.0f, 1.0f-(1-(getHealth()/100.0f)), 1.0f));
  m.loadToShader("material");


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

  trans.setRotation(0,-rot+90,0);

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

void Police::findTargetID(float _health)
{
    std::vector<int> rioters = getNeighbourRioterIDs();
    float currentRage = -1;
    Agent* currentTarget = NULL;
    for (int i=0; i<rioters.size(); i++)
    {
        Agent* rioter = dynamic_cast<Agent*>(m_entityMgr->getEntityFromID(rioters[i]));
        if (rioter)
        {
            if (rioter->getHealth()>_health && rioter->getRage()>currentRage)
            {
                currentRage = rioter->getRage();
                currentTarget = rioter;
            }
        }
    }

    if (currentTarget == NULL)
    {
        setTargetID(-1);
//        std::cout<< "NO NEARBY TARGETS"<<std::endl;
    }
    else
    {
        int target = currentTarget->getID();
        setTargetID(target);
//        std::cout<<"TARGET"<<target<<std::endl;
//        std::cout<< "FOUND TARGET"<<std::endl;
    }
}

void Police::checkValidPursuitRange(float _dist)
{
    ngl::Vec3 toSquad = m_pos - m_squadPos;
    double distSqFromEachOther = toSquad.lengthSquared();

    if (m_validPursuit)
    {
        if(distSqFromEachOther > _dist)
        {
            m_targetID = -1;
            m_validPursuit = false;
        }
    }

    else
    {
        m_validPursuit = true;
        if(distSqFromEachOther > _dist- (_dist/4.0))
        {
            m_targetID = -1;
            m_validPursuit = false;
        }
    }


}

void Police::findPathHome()
{
    m_homePos = m_world->getPoliceStation();
    findPath(m_homePos);
    m_hasPathHome = true;
}

bool Police::handleMessage(const Message& _message)
{
    switch(_message.m_message)
    {
    case msgAttack:
      m_health -= (_message.m_extraInfo * m_timeElapsed);
      return true;

    default:
      std::cout<<"Police: Message type not defined"<<std::endl;
      return false;
    }
}


//POLICE STATE UTILITY FUNCTIONS

void Police::attack()
{
  m_messageMgr->sendMessage(this->getID(), this->getTargetID(), msgAttack, m_damage);
}

void Police::death()
{
  for (int i = 0; i < m_world->getNumberOfRioters(); i++)
  {
    ngl::Vec3 vecToRioter = m_world->getRioters()[i]->getPos() - m_pos;
    double distSqToRioter = vecToRioter.lengthSquared();
    double affectedRadius = 8.0;
    if (distSqToRioter < affectedRadius * affectedRadius)
    {
      m_messageMgr->sendMessage(this->getID(), m_world->getRioters()[i]->getID(), msgPoliceDeath, 0.f);
    }
  }
}

void Police::squadCohesion(double weight)
{
    if(weight <= 0.0)
    {

      Vehicle::Steering()->SquadCohesionOff();
    }
    else
    {
        ngl::Vec3 toSquad = m_pos - m_squadPos;
        double distance = fabs(toSquad.length());

        weight = (weight*distance*1.5f)/m_squadRadius;


        Vehicle::setSquadCrosshair(m_squadPos);
        Vehicle::Steering()->setSquadCohesionWeight(weight);

        Vehicle::Steering()->SquadCohesionOn();
    }

}

void Police::registerClass(lua_State* _L)
{
  // Set up LUA state
  luaL_dofile(L, "lua/Police.lua");
  luaL_openlibs(L);

    registerLua(_L);
    luabridge::getGlobalNamespace(_L)
        .deriveClass<Police, Agent>("Police")
            .addConstructor <void (*) (GameWorld*, ngl::Obj*)> ()
                .addFunction("attack", &Police::attack)
                .addFunction("getRioterInfluence", &Police::getRioterInfluence)
                .addFunction("squadCohesion", &Police::squadCohesion)
                .addFunction("findPathHome", &Police::findPathHome)
                .addProperty("m_isMoving", &Police::getIsMoving, &Police::setIsMoving)
                .addFunction("checkValidPursuitRange", &Police::checkValidPursuitRange)
//                .addProperty("maxSpeed", &Police::Vehicle::getMaxSpeed, &Police::Vehicle::setMaxSpeed)

        .endClass();

    luabridge::push(L, this);
    lua_setglobal(L, "police");
}

