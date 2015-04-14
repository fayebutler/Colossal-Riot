

#include "GameWorld.h"



GameWorld::GameWorld()
{
  for (int i = 0; i < 10; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    m_rioters.push_back(newRioter);
  }

}
void GameWorld::Update(double timeElapsed)
{
    for(unsigned int a=0; a<m_rioters.size(); ++a)
    {
        Rioter* currentRioter = m_rioters[a];
        currentRioter->update(timeElapsed);
    }
}

void GameWorld::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{
  for(unsigned int a=0; a<m_rioters.size(); ++a)
  {
      Rioter* currentRioter = m_rioters[a];
      currentRioter->draw(cam, mouseGlobalTX);
  }
}
