//----------------------------------------------------------------------------------------------------------------------------
/// @file Rioter.cpp
/// @brief defines a rioter entity
//----------------------------------------------------------------------------------------------------------------------------

#include "StaticEntity.h"

//----------------------------------------------------------------------------------------------------------------------------
StaticEntity::StaticEntity(GameWorld* world,
                           ngl::Vec3 pos,
                           ngl::Vec3 dir,
                           float r,
                           obstacleType _type,
                           ngl::Obj *_mesh):BaseGameEntity(world, typeStaticEntity,pos,r)
{
  m_mesh = _mesh;
  m_obstacleType = _type;
  m_dir = dir;
}

//----------------------------------------------------------------------------------------------------------------------------
void StaticEntity::draw(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  loadMatricesToShader(cam, mouseGlobalTX);
  m_mesh->draw();
}

//----------------------------------------------------------------------------------------------------------------------------
void StaticEntity::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
      (*shader)["Phong"]->use();

  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;
  ngl::Transformation trans;

  if(m_obstacleType == obstacleTree)
  {
    ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
    m.setSpecularExponent(5.f);
    m.setDiffuse(ngl::Colour(0.0,0.5,0.0,1.0));

    m.loadToShader("material");

    trans.setPosition(m_pos);
    trans.setRotation(m_dir);
  }
  else if(m_obstacleType == obstacleStreetLight)
  {
    ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.2775f,0.2775f,0.2775f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
    m.setSpecularExponent(5.f);
    m.setDiffuse(ngl::Colour(0.6,0.6,0.6,1.0));

    m.loadToShader("material");

    trans.setPosition(m_pos);
    trans.setRotation(m_dir);
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
bool StaticEntity::handleMessage(const Message& _message)
{
  return BaseGameEntity::handleMessage(_message);
}

