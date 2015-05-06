#include "GameWorld.h"

#include <iostream>


GameWorld::GameWorld(int numberOfRioters)
{
   m_resetID = 1;

   m_mesh = new ngl::Obj("drawMesh.obj"); //Obj to draw, must be triangulated

   m_mesh->createVAO();

   m_entityMgr = new EntityManager();
   m_cellGraph = CellGraph("navMesh.obj"); //Obj for cell graph, must be quads
   m_cellGraph.generateWalls();


  for (int i = 0; i < numberOfRioters ; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    newRioter->setBoudingRadius(0.5f);
    newRioter->setDetectionRadius(3.5f);
    newRioter->setHeading(ngl::Vec3(-1+2*((float)rand())/RAND_MAX, 0.f, -1+2*((float)rand())/RAND_MAX));
    newRioter->setPos(ngl::Vec3(-25+50*((float)rand())/RAND_MAX, 0.f, -25+50*((float)rand())/RAND_MAX));
    m_cellGraph.initializeCells(m_entityMgr->getEntityFromID(newRioter->getID()));
    while (newRioter->getCurrentCellID() < 0)
    {
      newRioter->setPos(ngl::Vec3(-50+100*((float)rand())/RAND_MAX, 0.f, -50+100*((float)rand())/RAND_MAX));
      m_cellGraph.initializeCells(m_entityMgr->getEntityFromID(newRioter->getID()));
    }
    m_rioters.push_back(newRioter);
//    std::cout<<"RIOTER ID: "<<newRioter->getID()<<std::endl;
  }
//  for (int i = 0; i < 1 ; ++i)
//  {
//    Police* newRioter = new Police(this);
//    newRioter->setBoudingRadius(0.5f);
//    newRioter->setDetectionRadius(3.5f);
//    newRioter->setHeading(ngl::Vec3(-1+2*((float)rand())/RAND_MAX, 0.f, -1+2*((float)rand())/RAND_MAX));
//    newRioter->setPos(ngl::Vec3(2,0,2));
//    m_cellGraph.initializeCells(m_entityMgr->getEntityFromID(newRioter->getID()));
//    while (newRioter->getCurrentCellID() < 0)
//    {
//      newRioter->setPos(ngl::Vec3(2,0,2));
//      m_cellGraph.initializeCells(m_entityMgr->getEntityFromID(newRioter->getID()));
//    }
//    m_police.push_back(newRioter);
//  }


//  for (int i = 0; i < 1; ++i)
//  {

//      Squad* newSquad = new Squad(this, 6, ngl::Vec3(10.0f,0.0f,8.0f), 1.f);
//      m_squads.push_back(newSquad);
//  }

    m_numberOfEntities = m_entityMgr->getSize();

  for (unsigned int i=0; i<m_numberOfEntities; i++)
  {
      //Adds entities to cells and cell ID to entities
      m_cellGraph.initializeCells(m_entityMgr->getEntityFromID(i));
  }
  m_numberOfEntities = m_entityMgr->getSize();
  m_numberOfRioters = m_rioters.size();
  //m_numberOfPolice = m_police.size();

}

GameWorld::~GameWorld()
{
   m_rioters.clear();
   m_police.clear();
   m_squads.clear();
   delete m_mesh;
   delete m_entityMgr;
}

void GameWorld::Update(double timeElapsed, double currentTime)
{
    //Clear the cells of agentIDs

  /// don't call everyframe -------------------

    m_cellGraph.clearCells();

    m_numberOfEntities = m_entityMgr->getSize();
    m_numberOfRioters = m_rioters.size();
    m_numberOfPolice = m_police.size();

    for (unsigned int i=0; i<m_numberOfEntities; i++)
    {
        //Adds entities to cells and cell ID to entities
        ///needs to find ith ID in entity map
        m_cellGraph.updateCells(m_entityMgr->getEntityFromID(i));
    }

    for (unsigned int i=0; i<m_numberOfEntities; i++)
    {
        //Adds entity neighbours:
      if (m_entityMgr->getEntityFromID(i)->getCurrentCellID() >= 0)
      {
        m_cellGraph.addEntities(m_entityMgr->getEntityFromID(i));
      }
    }
//    std::vector<ngl::Vec3> path = m_cellGraph.findPath(m_entityMgr->getEntityFromID(0), ngl::Vec3 (10, 0, -9));


    /// ----------------------------------------------------

    //(WHEN MAKING CELLS THEY NEED TO HAVE VECTORS OF ALL STATIC ENTITIES (walls n shit))

    //1. for each agent -> updateCells
    // finds the cell agent is in; adds agentID to cell and cellID to agent.

    //2. for each moving entity (csGas, molotov etc.) -> updateCells
    // finds the cell moving entity is in; adds ID to cell and cellID to movingEntity.

    //3. for each agent -> get Neighbouring everything (other agents, moving entities, walls, obstacles)
    // get all the IDs from current and all neighbouring cells; check if within detectionRadius
    // add to one vector of IDs? seperate vectors? when to we distinguish? Cell level? ahhhhhhhhhh

    //4. same loop? update agent.

/// ---------------------------------------------------------------------------------


    for(unsigned int a=0; a<m_numberOfRioters; ++a)
    {
        Rioter* currentRioter = m_rioters[a];
        currentRioter->update(timeElapsed, currentTime);

    }


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
    Squad* newSquad = new Squad(this, size, ngl::Vec3(13.0f,0.0f,5.0f), 0.5f);
    m_squads.push_back(newSquad);

    m_numberOfEntities = m_entityMgr->getSize();

      //Adds entities to cells and cell ID to entities
    for (unsigned int i=m_numberOfEntities-(size+1); i<m_numberOfEntities; i++)
    {
        //Adds entities to cells and cell ID to entities
        m_cellGraph.initializeCells(m_entityMgr->getEntityFromID(i));
    }

    m_cellGraph.initializeCells(m_entityMgr->getEntityFromID(newSquad->getID()));
}

void GameWorld::createPath(Squad* selectedSquad, ngl::Vec3 target)
{

    selectedSquad->getPath().clear();
    std::vector<ngl::Vec3> path = m_cellGraph.findPath(m_entityMgr->getEntityFromID(selectedSquad->getID()), target);

    selectedSquad->setPath(path);
}

bool GameWorld::loseGame()
{
    int numberDeadPolice = 0;
    for(int i =0; i < m_numberOfPolice; i++)
    {
        Police* policeMan = m_police[i];
        if(policeMan->getHealth() == 0.0)
        {
            numberDeadPolice += 1;
        }

    }
//    std::cout<<"number of police = "<<m_numberOfPolice<<std::endl;
//    std::cout<<"dead police = "<<numberDeadPolice<<std::endl;
    if(m_numberOfPolice > 0 &&  numberDeadPolice == m_numberOfPolice)
    {
//        std::cout<<" YOU LOOOOSEEEE"<<std::endl;
        return true;
    }
    else
    {
        return false;
    }
}
