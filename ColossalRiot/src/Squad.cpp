#include "Squad.h"

ngl::Vec3 Squad::s_nextSelectionColour = ngl::Vec3(0.0,0.0,0.0);

Squad::Squad(GameWorld* world, int squadSize, ngl::Vec3 pos, float r):BaseGameEntity(typeSquad,pos,r)
{
//    m_squadPos = ngl::Vec3(2.0,0,2.0);
    //m_squadPos = pos;

    //m_boundingRad = r;
    m_squadColour = ngl::Colour(1.0f,0.0f,0.0f,1.0f);

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

    m_selectionColour = s_nextSelectionColour;

    s_nextSelectionColour.m_x += 0.1;

    if(s_nextSelectionColour.m_x == 1)
    {
        s_nextSelectionColour.m_x = 0;
        s_nextSelectionColour.m_y += 0.1;

        if(s_nextSelectionColour.m_y == 1)
        {
            s_nextSelectionColour.m_y = 0;
            s_nextSelectionColour.m_z += 0.1;
        }
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
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
      (*shader)["Phong"]->use();

    ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
    m.setSpecularExponent(5.f);
    m.setDiffuse(ngl::Colour(m_squadColour));
    m.loadToShader("material");

    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;
    ngl::Transformation trans;
    trans.setPosition(m_pos.m_x,m_pos.m_y, m_pos.m_z);
    trans.setRotation(90.0,0.0,0.0);


    M=trans.getMatrix()*mouseGlobalTX;
    MV=  M*cam->getViewMatrix();
    MVP= M*cam->getVPMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
//    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
//    shader->setShaderParamFromMat4("M",M);

}

bool Squad::checkSelectionColour(const ngl::Vec3 colour)
{
    if(m_selectionColour.m_x >= colour.m_x-0.01f && m_selectionColour.m_x <= colour.m_x+0.01f &&
            m_selectionColour.m_y >= colour.m_y-0.01f && m_selectionColour.m_y <= colour.m_y+0.01f &&
            m_selectionColour.m_z >= colour.m_z-0.01f && m_selectionColour.m_z <= colour.m_z+0.01f)
    {
        std::cout<<"MATCHES!!";
        std::cout<<"SQUAD ID "<<getID()<<std::endl;

        return true;
    }
    else
    {
        std::cout<<"DOESNT MATCH!";
        std::cout<<"SQUAD ID "<<getID()<<std::endl;
        return false;
    }
}

void Squad::selectionDraw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["nglColourShader"]->use();
    shader->setShaderParam4f("Colour", m_selectionColour.m_x, m_selectionColour.m_y,m_selectionColour.m_z, 1);

    ngl::Transformation t;
    t.setPosition(m_pos);
    t.setRotation(90.0,0.0,0.0);

    ngl::Mat4 M;
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
//    MV=t.getMatrix()*mouseGlobalTX*cam->getViewMatrix() ;
//    MVP=MV*cam->getVPMatrix();
//    shader->setShaderParamFromMat4("MVP",MVP);

    M=t.getMatrix()*mouseGlobalTX;
    MV=  M*cam->getViewMatrix();
    MVP= M*cam->getVPMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
//    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
//    shader->setShaderParamFromMat4("M",M);
    ngl::VAOPrimitives::instance()->createDisk("squad",m_squadRadius,120);
    ngl::VAOPrimitives::instance()->draw("squad");


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
