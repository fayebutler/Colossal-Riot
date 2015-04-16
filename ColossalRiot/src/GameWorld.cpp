#include "GameWorld.h"

#include <iostream>



GameWorld::GameWorld()
{
  for (int i = 0; i < 5; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    newRioter->setPos(ngl::Vec3(5.0, 0.0, 0.0));
    m_rioters.push_back(newRioter);

  }
  for (int i = 0; i < 2; ++i)
  {
    Police* newPolice = new Police(this);
    m_police.push_back(newPolice);
  }


}
void GameWorld::Update(double timeElapsed)
{
    for(unsigned int a=0; a<m_rioters.size(); ++a)
    {
        Rioter* currentRioter = m_rioters[a];
        currentRioter->update(timeElapsed);
    }
    for(unsigned int a=0; a<m_police.size(); ++a)
    {
        Police* currentPolice = m_police[a];
        currentPolice->update(timeElapsed);
        std::cout<<currentPolice->getHealth()<<std::endl;
    }
}

void GameWorld::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{
  for(unsigned int a=0; a<m_rioters.size(); ++a)
  {
      Rioter* currentRioter = m_rioters[a];
      currentRioter->draw(cam, mouseGlobalTX);
  }
  for(unsigned int a=0; a<m_police.size(); ++a)
  {
      Police* currentPolice = m_police[a];
      currentPolice->draw(cam, mouseGlobalTX);
  }
}
