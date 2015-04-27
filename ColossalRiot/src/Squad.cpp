#include "Squad.h"

Squad::Squad(GameWorld* world, int squadSize, ngl::Vec3 pos)
{
//    m_squadPos = ngl::Vec3(2.0,0,2.0);
    m_squadPos = pos;

    float boundingRad = 1.5f;

    m_squadRadius = squadSize*boundingRad/2.0f;

    for (int i = 0; i < squadSize; ++i)
    {
      Police* newPolice = new Police(world);
      newPolice->setBoudingRadius(boundingRad);
      newPolice->setDetectionRadius(10.f);
      newPolice->setPos(ngl::Vec3((((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_squadPos.m_x, 0.0f, (((float)rand()/RAND_MAX)*m_squadRadius*2)-m_squadRadius+ m_squadPos.m_z));
      newPolice->setSquadPos(m_squadPos);
      newPolice->setSquadRadius(m_squadRadius);
      m_squadPolice.push_back(newPolice);

    }
}

void Squad::update(double timeElapsed, double currentTime)
{
    for(unsigned int i=0; i<m_squadPolice.size(); ++i)
    {
        Police* currentPolice = m_squadPolice[i];
        currentPolice->setSquadPos(m_squadPos);
        currentPolice->setSquadRadius(m_squadRadius);
        currentPolice->update(timeElapsed, currentTime);
    }

    if (currentTime > 40.0 && currentTime < 40.17)
    {
        setSquadPos(getSquadPos()+ngl::Vec3(1.0,0.0,1.0));
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
    trans.setPosition(m_squadPos);
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
