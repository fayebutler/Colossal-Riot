#include "GameWorld.h"

#include <iostream>



GameWorld::GameWorld()
{
  for (int i = 0; i < 2; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    newRioter->setPos(ngl::Vec3(-3.0, 0.0, 3.0));
    std::cout<<"rioterid"<<newRioter->getID()<<std::endl;
    m_rioters.push_back(newRioter);

  }
  for (int i = 0; i < 1; ++i)
  {
    Police* newPolice = new Police(this);
    //newPolice->setTargetID(i);

    newPolice->setPos(ngl::Vec3(2.0, 0.0, -1.0));
    std::cout<<"policeid"<<newPolice->getID()<<std::endl;
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
