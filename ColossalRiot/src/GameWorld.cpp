

#include "GameWorld.h"



GameWorld::GameWorld()
{
  for (int i = 0; i < 5; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    m_rioters.push_back(newRioter);
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
