#include "Squad.h"

Squad::Squad(GameWorld* world, int squadSize, ngl::Vec3 pos, float r):BaseGameEntity(typeSquad,pos,r)
{
//    m_squadPos = ngl::Vec3(2.0,0,2.0);
    //m_squadPos = pos;

    //m_boundingRad = r;

    m_squadRadius = squadSize*m_boundingRadius/2.0f;

    for (int i = 0; i < squadSize; ++i)
    {
      Police* newPolice = new Police(world);
      newPolice->setBoudingRadius(m_boundingRadius);
      newPolice->setDetectionRadius(5.f);
      newPolice->setPos(ngl::Vec3((((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_pos.m_x, 0.0f, (((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_pos.m_z));
      newPolice->setSquadPos(m_pos);
      newPolice->setSquadRadius(m_squadRadius);
      newPolice->setSquadID(m_ID);
      m_squadPolice.push_back(newPolice);

    }
}

void Squad::update(double timeElapsed, double currentTime)
{
    for(unsigned int i=0; i<m_squadPolice.size(); ++i)
    {
        Police* currentPolice = m_squadPolice[i];
        currentPolice->setSquadPos(m_pos);
        currentPolice->setSquadRadius(m_squadRadius);
        currentPolice->update(timeElapsed, currentTime);
    }

    if (currentTime > 40.0 && currentTime < 40.17)
    {
        setPos(getPos()+ngl::Vec3(1.0,0.0,1.0));
    }
}

void Squad::draw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
    for(unsigned int i=0; i<m_squadPolice.size(); ++i)
    {
        Police* currentPolice = m_squadPolice[i];
        currentPolice->draw(cam, mouseGlobalTX);
    }

    loadMatricesToShader(cam, mouseGlobalTX);

    ngl::VAOPrimitives::instance()->createDisk("squad",m_squadRadius,120);
    ngl::VAOPrimitives::instance()->draw("squad");

}

void Squad::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{

    ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
    m.setSpecularExponent(5.f);
    m.setDiffuse(ngl::Colour(0.0f, 0.7f, 0.2f, 1.0f));
    m.loadToShader("material");

    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    ngl::Transformation trans;
    trans.setPosition(m_pos);
    trans.setRotation(90.0,0.0,0.0);


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

bool Squad::handleMessage(const Message& _message)
{
  switch(_message.m_message)
  {
  case msgAttack:

    return true;

  default:
    std::cout<<"Agent: Message type not defined"<<std::endl;
    return false;
  }
}
