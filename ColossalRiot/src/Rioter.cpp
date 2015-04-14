#include "Rioter.h"

Rioter::Rioter(GameWorld* world) : Agent(world)
{
    // Set up LUA state
    luaL_dofile(L, "lua/Rioter.lua");
    luaL_openlibs(L);

    registerClass(L);
    luabridge::push(L, this);
    lua_setglobal(L, "rioter");

    // Set up state machine
    m_stateMachine = new StateMachine<Rioter>(this);
//    m_stateMachine->setGlobalState(Class::Instance());

    // Set initial variables
    m_targetID = 0;
    luabridge::LuaRef makeRioter = luabridge::getGlobal(L, "makeRioter");
    makeRioter();

}

Rioter::~Rioter()
{
  lua_close(L);
  delete m_stateMachine; 
}

void Rioter::update(double timeElapsed)
{
    Agent::update(timeElapsed);
    m_stateMachine->update();
}

void Rioter::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{
  loadMatricesToShader(cam, mouseGlobalTX);
  ngl::VAOPrimitives::instance()->draw("teapot");
}

void Rioter::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  ngl::Transformation trans;
  trans.setPosition(getVehicle()->getPos());
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

//bool Rioter::handleMessage(const Message& _message)
//{
//  Agent::handleMessage(_message);
//}


//RIOTER STATE UTILITY FUNCTIONS

void Rioter::attack(int _ID)
{
  std::cout<<"RIOT RIOT Attack: "<<_ID<<" for "<<m_rage<<std::endl;
  MessageMgr->sendMessage(this->getID(),m_targetID,msgAttack,0,m_rage);
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
