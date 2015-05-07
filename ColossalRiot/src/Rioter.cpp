#include "Rioter.h"

Rioter::Rioter(GameWorld* world) : Agent(world)
{
    m_messageMgr = new MessageManager();
    m_entityType = typeRioter;

    // Set up LUA state
    luaL_dofile(L, "lua/Rioter.lua");
    luaL_openlibs(L);

    registerClass(L);
    luabridge::push(L, this);
    lua_setglobal(L, "rioter");

    // Set up state machine
    m_stateMachine = new StateMachine<Rioter>(this);

    // Set initial variables

    m_hopHeight = 0.0;
    m_hopSpeed = 0.0;
    luabridge::LuaRef makeRioter = luabridge::getGlobal(L, "makeRioter");
    makeRioter();

    //Vehicle::Steering()->ObstacleAvoidOn();

//    Vehicle::Steering()->CohesionOn();
//    Vehicle::Steering()->setCohesionWeight(0.2f);

//    Vehicle::Steering()->AlignmentOn();
//    Vehicle::Steering()->setAlignmentWeight(0.5f);


//    Vehicle::Steering()->SeparationOn();
//    Vehicle::Steering()->setSeparationWeight(1.0f);

    Vehicle::setMaxSpeed(5);

    Vehicle::Steering()->WallAvoidOn();

}

Rioter::~Rioter()
{
  lua_close(L);
  delete m_stateMachine;
  delete m_messageMgr;
}

void Rioter::update(double timeElapsed, double currentTime)
{
    Vehicle::Steering()->clearFriendlyNeighbours();
    Vehicle::Steering()->clearAllNeighbours();
    Vehicle::Steering()->addFriendlyNeighbours(getNeighbourRioterIDs());
    Vehicle::Steering()->addAllNeighbours(getNeighbourRioterIDs());
    Vehicle::Steering()->addAllNeighbours(getNeighbourPoliceIDs());

    Agent::update(timeElapsed, currentTime);
    m_stateMachine->update();
//    m_hopSpeed += (m_rage/50.0) - (m_health/50.0);
    m_hop = (sin((currentTime*m_hopSpeed)+m_ID)*sin((currentTime*m_hopSpeed)+m_ID)*m_hopHeight);

    Vehicle::Steering()->WallOverlapAvoidance();
    Vehicle::Steering()->ObjectOverlapAvoidance();
}

void Rioter::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{
  loadMatricesToShader(cam, mouseGlobalTX);
  ngl::VAOPrimitives::instance()->draw("cube");

}

void Rioter::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
  m.setSpecularExponent(5.f);
  m.setDiffuse(ngl::Colour(getHealth()/100.0f, getHealth()/100.0f*0.4, getHealth()/100.0f*0.01, 1.0f));
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

void Rioter::findTargetID(float _health)
{
    std::vector<int> police = getNeighbourPoliceIDs();
    float currentHealth = 0;
    Agent* currentTarget = NULL;
    for (int i=0; i<police.size(); i++)
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
//        std::cout<< "NO NEARBY TARGETS"<<std::endl;
    }
    else
    {
        int target = currentTarget->getID();
        setTargetID(target);
//        std::cout<< "FOUND TARGET"<<std::endl;
    }
}

bool Rioter::handleMessage(const Message& _message)
{
  return Agent::handleMessage(_message);
}


//RIOTER STATE UTILITY FUNCTIONS

void Rioter::attack()
{

  m_messageMgr->sendMessage(this->getID(),this->getTargetID(),msgAttack,0,m_damage);


}

void Rioter::registerClass(lua_State* _L)
{
    registerLua(_L);
    luabridge::getGlobalNamespace(_L)
        .deriveClass<Rioter, Agent>("Rioter")
            .addConstructor <void (*) (GameWorld*)> ()
                .addFunction("attack", &Rioter::attack)
                .addFunction("findTargetID", &Rioter::findTargetID)
        .endClass();
}
