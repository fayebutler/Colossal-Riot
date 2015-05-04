#include "Police.h"
#include <math.h>

Police::Police(GameWorld* world) : Agent(world)
{
  m_messageMgr = new MessageManager();

    m_entityType = typePolice;

    // Set up LUA state
    luaL_dofile(L, "lua/Police.lua");
    luaL_openlibs(L);

    registerClass(L);
    luabridge::push(L, this);
    lua_setglobal(L, "police");

    // Set up state machine
    m_stateMachine = new StateMachine<Police>(this);

    // Set initial variables

    m_enRoute = false;

    m_hopHeight = 0.0;
    m_hopSpeed = 0.0;
    luabridge::LuaRef makePolice = luabridge::getGlobal(L, "makePolice");
    makePolice();

    //m_targetID = 0;

//    Vehicle::Steering()->WanderOn();
//    Vehicle::Steering()->setWanderWeight(0.5);

  //  Vehicle::Steering()->ObstacleAvoidOn();

//    Vehicle::Steering()->CohesionOn();
//    Vehicle::Steering()->setCohesionWeight(0.8f);

//    Vehicle::Steering()->AlignmentOn();
//    Vehicle::Steering()->setAlignmentWeight(0.3f);


    Vehicle::Steering()->SeparationOn();
    Vehicle::Steering()->setSeparationWeight(0.6f);


    Vehicle::Steering()->WallAvoidOn();
    Vehicle::Steering()->setWallAvoidWeight(0.2);
}

Police::~Police()
{
  lua_close(L);
  delete m_stateMachine;
  delete m_messageMgr;
}

void Police::update(double timeElapsed, double currentTime)
{
  Agent::update(timeElapsed, currentTime);
  m_stateMachine->update();

  m_hop = (sin(currentTime*m_hopSpeed)*sin(currentTime*m_hopSpeed)*m_hopHeight);

  //make a friendly and opposing neightbours vector as seperation wants all neighbours bu alignment & cohesion doesn't
  Vehicle::Steering()->clearFriendlyNeighbours();
  Vehicle::Steering()->clearAllNeighbours();
  Vehicle::Steering()->addFriendlyNeighbours(getNeighbourPoliceIDs());
  Vehicle::Steering()->addAllNeighbours(getNeighbourRioterIDs());
  Vehicle::Steering()->addAllNeighbours(getNeighbourPoliceIDs());

  Vehicle::Steering()->WallOverlapAvoidance();
  Vehicle::Steering()->ObjectOverlapAvoidance();

  Vehicle::setMaxSpeed(0.8);

}


void Police::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{
  loadMatricesToShader(cam, mouseGlobalTX);
  ngl::VAOPrimitives::instance()->draw("cube");
}

void Police::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
      (*shader)["Phong"]->use();

  ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
  m.setSpecularExponent(5.f);
  m.setDiffuse(ngl::Colour(0.0f, 0.0f, getHealth()/100.0f, 1.0f));
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

  trans.setRotation(0,-rot,0);

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
    float currentRage = -100;
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
//        std::cout<< "FOUND TARGET"<<std::endl;
    }
}

bool Police::handleMessage(const Message& _message)
{
  return Agent::handleMessage(_message);
}


//POLICE STATE UTILITY FUNCTIONS

void Police::attack()
{

  m_messageMgr->sendMessage(this->getID(),this->getTargetID(),msgAttack,0,m_damage);
}

void Police::registerClass(lua_State* _L)
{
    registerLua(_L);
    luabridge::getGlobalNamespace(_L)
        .deriveClass<Police, Agent>("Police")
            .addConstructor <void (*) (GameWorld*)> ()
                .addFunction("attack", &Police::attack)
                .addFunction("findTargetID", &Police::findTargetID)
                .addFunction("squadCohesion", &Police::squadCohesion)
                .addProperty("m_enRoute", &Police::getEnRoute, &Police::setEnRoute)
        .endClass();
}

void Police::squadCohesion(double weight)
{
    ngl::Vec3 toSquad = Vehicle::getPos() - m_squadPos;
    double distance = fabs(toSquad.length());

    weight = (weight*distance*1.5f)/m_squadRadius;


    Vehicle::setCrosshair(m_squadPos);
    Vehicle::Steering()->setSeekWeight(weight);

    Vehicle::Steering()->SeekOn();

}
