#include "GameWorld.h"

#include <iostream>



GameWorld::GameWorld()
{
  for (int i = 0; i < 1; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    newRioter->setPos(ngl::Vec3(0.0, 0.0, 0.0));
    m_rioters.push_back(newRioter);

  }
  for (int i = 0; i < 2; ++i)
  {
    Police* newPolice = new Police(this);
    //newPolice->setTargetID(i);
    newPolice->setPos(ngl::Vec3(2.0, 0.0, 2.0));
    m_police.push_back(newPolice);
  }

  Police* policeman = m_police[0];
  Rioter* riotman = m_rioters[0];
  policeman->Steering()->PursuitOn();
  policeman->Steering()->setTargetAgent(riotman);
  //riotman->Steering()->setTargetAgent(policeman);
  //riotman->Steering()->EvadeOn();
  riotman->Steering()->WanderOn();


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
