#include "GameWorld.h"

#include <iostream>


GameWorld::GameWorld(int numberOfRioters, int availablePolice)
{
   m_availablePolice = availablePolice;
   m_activePolice = 0;

   m_resetID = 1;

   m_mesh = new ngl::Obj("drawMesh.obj"); //Obj to draw, must be triangulated

   m_mesh->createVAO();

   m_entityMgr = new EntityManager();

   m_cellGraph = new CellGraph("navMesh.obj"); //Obj for cell graph, must be quads
   m_cellGraph->generateWalls();



  for (int i = 0; i < numberOfRioters ; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    newRioter->setBoudingRadius(0.5f);
    newRioter->setDetectionRadius(3.5f);
    newRioter->setHeading(ngl::Vec3(-1+2*((float)rand())/RAND_MAX, 0.f, -1+2*((float)rand())/RAND_MAX));
    newRioter->setPos(ngl::Vec3(-25+50*((float)rand())/RAND_MAX, 0.f, -25+50*((float)rand())/RAND_MAX));
    m_cellGraph->initializeCells(m_entityMgr->getEntityFromID(newRioter->getID()));
    while (newRioter->getCurrentCellID() < 0)
    {
      newRioter->setPos(ngl::Vec3(-50+100*((float)rand())/RAND_MAX, 0.f, -50+100*((float)rand())/RAND_MAX));
      m_cellGraph->initializeCells(m_entityMgr->getEntityFromID(newRioter->getID()));

    }
    m_rioters.push_back(newRioter);
//    std::cout<<"RIOTER ID: "<<newRioter->getID()<<std::endl;
  }



    m_numberOfRioters = m_rioters.size();


}

GameWorld::~GameWorld()
{
   m_rioters.clear();
   m_squads.clear();
   delete m_mesh;
   delete m_entityMgr;
}

void GameWorld::Update(double timeElapsed, double currentTime)
{
    //check for deaths
    m_numberOfSquads = m_squads.size();

    for(int i=0; i<m_numberOfSquads; i++)
    {
        m_squads[i]->checkDeaths();
    }

    m_numberOfRioters = m_rioters.size();

    for(int i=0; i<m_numberOfRioters; i++)
    {
        Rioter* currentRioter = m_rioters[i];
        if(currentRioter->getHealth()<=0)
        {
            m_entityMgr->removeEntity(dynamic_cast<BaseGameEntity*>(currentRioter));
            delete currentRioter;
            m_rioters.erase(m_rioters.begin()+i);
            m_numberOfRioters -= 1;
            std::cout<<"REMOVING RIOTER "<<i<<" EntityMap Size: "<<m_entityMgr->getSize()<<std::endl;
            i--;
        }
    }

    //check for empty squads

    for(int i=0; i<m_squads.size(); i++)
    {
        std::cout<<"SQUAD SIZE "<<m_squads[i]->getSquadSize()<<std::endl;
        if (m_squads[i]->getSquadSize() <= 0)
        {
            m_entityMgr->removeEntity(m_squads[i]);
            delete m_squads[i];
            m_squads.erase(m_squads.begin()+i);
            std::cout<<"deleted squad, m_squad size: "<<m_squads.size()<<std::endl;
        }
    }



    m_cellGraph->clearCells();
    std::map<int,BaseGameEntity*> myMap = m_entityMgr->getEntityMap();

    for (std::map<int,BaseGameEntity*>::const_iterator it = myMap.begin(); it != myMap.end(); ++it)
    {

        //Adds entities to cells and cell ID to entities
        m_cellGraph->updateCells(it->second);
    }

    for (std::map<int,BaseGameEntity*>::const_iterator it = myMap.begin(); it != myMap.end(); ++it)
    {
        //Adds entity neighbours:
      if (it->second->getCurrentCellID() >= 0)
      {
        m_cellGraph->addEntities(it->second);
      }
    }


    // call rioter and squad updates

    m_numberOfRioters = m_rioters.size();

    std::cout<<"updating rioters"<<std::endl;
    for(unsigned int a=0; a<m_numberOfRioters; ++a)
    {
        Rioter* currentRioter = m_rioters[a];
        currentRioter->update(timeElapsed, currentTime);

    }


    m_numberOfSquads = m_squads.size();

    std::cout<<"updating squads"<<std::endl;
    for(unsigned int a=0; a<m_squads.size(); ++a)
    {
        Squad* currentSquad = m_squads[a];
        currentSquad->update(timeElapsed, currentTime);
    }
}

void GameWorld::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{

//  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.1f,0.5f,0.0f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
  m.setSpecularExponent(5.f);
  m.loadToShader("material");

  ngl::ShaderLib *shader=ngl::ShaderLib::instance();


  ngl::Mat4 MV;
  ngl::Mat4 MVP;
  ngl::Mat3 normalMatrix;
  ngl::Mat4 M;

  M=mouseGlobalTX;
  MV=  M*cam->getViewMatrix();
  MVP= M*cam->getVPMatrix();
  normalMatrix=MV;
  normalMatrix.inverse();
  shader->setShaderParamFromMat4("MV",MV);
  shader->setShaderParamFromMat4("MVP",MVP);
  shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
  shader->setShaderParamFromMat4("M",M);

}


void GameWorld::draw(ngl::Camera* cam, ngl::Mat4 mouseGlobalTX)
{

  loadMatricesToShader(cam, mouseGlobalTX);
  m_mesh->draw();
  for(unsigned int a=0; a<m_numberOfRioters; ++a)
  {
      Rioter* currentRioter = m_rioters[a];
      currentRioter->draw(cam, mouseGlobalTX);
  }

  for(unsigned int a=0; a<m_squads.size(); ++a)
  {
      Squad* currentSquad = m_squads[a];
      currentSquad->draw(cam, mouseGlobalTX);
  }
}

void GameWorld::createSquad(int size)
{
    if (m_availablePolice - size < 0)
    {
        std::cout<<"Not enough police available!"<<std::endl;
    }
    else
    {
        Squad* newSquad = new Squad(this, size, ngl::Vec3(13.0f,0.0f,5.0f), 0.5f);
        m_squads.push_back(newSquad);

        m_activePolice += size;
        m_availablePolice -= size;
    }
}

void GameWorld::createPath(Squad* selectedSquad, ngl::Vec3 target)
{

    std::vector<ngl::Vec3> path = m_cellGraph->findPath(m_entityMgr->getEntityFromID(selectedSquad->getID()), target);
    selectedSquad->setPath(path);
}


