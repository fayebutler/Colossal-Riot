#include "Rioter.h"

Rioter::Rioter(GameWorld* world) : Agent(world)
{
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

    m_targetID = 0;

    Vehicle::Steering()->WanderOn();
    //Vehicle::Steering()->SeekOn();
    //setCrosshair(ngl::Vec3(5.f, 0.f, 1.f));
    //Vehicle::Steering()->ObstacleAvoidOn();

    Vehicle::Steering()->CohesionOn();
    Vehicle::Steering()->setCohesionWeight(1.f);

    Vehicle::Steering()->AlignmentOn();
    Vehicle::Steering()->setAlignmentWeight(1.f);

    Vehicle::Steering()->SeparationOn();
    Vehicle::Steering()->setSeparationWeight(1.f);

}

Rioter::~Rioter()
{
  lua_close(L);
  delete m_stateMachine; 
}

void Rioter::update(double timeElapsed, double currentTime)
{
    Agent::update(timeElapsed, currentTime);
    m_stateMachine->update();

    m_hop = (sin(currentTime*m_hopSpeed)*sin(currentTime*m_hopSpeed)*m_hopHeight);

    Vehicle::Steering()->clearNeighbours();
    Vehicle::Steering()->addNeighbours(getNeighbourPoliceIDs());
    Vehicle::Steering()->addNeighbours(getNeighbourRioterIDs());

}

void Rioter::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{
  loadMatricesToShader(cam, mouseGlobalTX);
  ngl::VAOPrimitives::instance()->draw("teapot");

}

void Rioter::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
  m.setSpecularExponent(5.f);
  m.setDiffuse(ngl::Colour(getRage()/100.0f, 0.0f, 0.0f, 1.0f));
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

bool Rioter::handleMessage(const Message& _message)
{
  return Agent::handleMessage(_message);
}


//RIOTER STATE UTILITY FUNCTIONS

void Rioter::attack()
{

  MessageMgr->sendMessage(this->getID(),this->getTargetID(),msgAttack,0,m_damage);


}

void Rioter::registerClass(lua_State* _L)
{
    registerLua(_L);
    luabridge::getGlobalNamespace(_L)
        .deriveClass<Rioter, Agent>("Rioter")
            .addConstructor <void (*) (GameWorld*)> ()
                .addFunction("attack", &Rioter::attack)
        .endClass();
}
