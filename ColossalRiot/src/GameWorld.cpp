#include "GameWorld.h"

#include <iostream>



GameWorld::GameWorld()
{
  for (int i = 0; i < 5; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    newRioter->setPos(ngl::Vec3(-7+14*((float)rand())/RAND_MAX, 0.f, -7+14*((float)rand())/RAND_MAX));
    newRioter->setBoudingRadius(3.f);
    //newRioter->setCrosshair(ngl::Vec3(-4.f, 0.f, 0.f));
    m_rioters.push_back(newRioter);

  }
  for (int i = 0; i < 5; ++i)
  {
    Police* newPolice = new Police(this);
    newPolice->setBoudingRadius(3.f);
    newPolice->setPos(ngl::Vec3(-7+14*((float)rand())/RAND_MAX, 0.f, -7+14*((float)rand())/RAND_MAX));
    newPolice->setCrosshair(ngl::Vec3(4.f, 0.f, 0.f));
    m_police.push_back(newPolice);
  }
}

void GameWorld::Update(double timeElapsed, double currentTime)
{
    for(unsigned int a=0; a<m_rioters.size(); ++a)
    {
        Rioter* currentRioter = m_rioters[a];
        currentRioter->update(timeElapsed, currentTime);

    }
    for(unsigned int a=0; a<m_police.size(); ++a)
    {
        Police* currentPolice = m_police[a];
        currentPolice->update(timeElapsed, currentTime);
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
