#include "GameWorld.h"

#include <iostream>




GameWorld::GameWorld()
{


    m_mesh = new ngl::Obj("plane_Mesh.obj"); //Obj to draw, must be triangulated
    m_mesh->createVAO();

   m_entityMgr = new EntityManager();
   m_cellGraph =  CellGraph("plane_Test.obj"); //Obj for cell graph, must be quads
   m_cellGraph.generateWalls();




  for (int i = 0; i < 8; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    newRioter->setBoudingRadius(2.f);
    newRioter->setDetectionRadius(10.f);
    newRioter->setPos(ngl::Vec3(-7+14*((float)rand())/RAND_MAX, 0.f, -7+14*((float)rand())/RAND_MAX));
    m_rioters.push_back(newRioter);


  }
  for (int i = 0; i < 8; ++i)
  {
    Police* newPolice = new Police(this);
    newPolice->setBoudingRadius(2.f);
    newPolice->setDetectionRadius(10.f);
    newPolice->setPos(ngl::Vec3(-7+14*((float)rand())/RAND_MAX, 0.f, -7+14*((float)rand())/RAND_MAX));
    m_police.push_back(newPolice);
  }

}

void GameWorld::Update(double timeElapsed, double currentTime)
{
//    void * edpp = EntityMgr->getEntityFromID(0);
//    if (edpp->getEntityType() == typeRioter)  m_mesh->draw();


    //Clear the cells of agentIDs

    m_cellGraph.clearCells();
    m_cellGraph.printCellGraph();

    for (int i=0; i<m_entityMgr->getEntityMap().size(); i++)
    {
        //Adds entities to cells and cell ID to entities
        m_cellGraph.updateCells(m_entityMgr->getEntityFromID(i));
    }

    for (int i=0; i<m_entityMgr->getEntityMap().size(); i++)
    {
        //Adds entity neighbours:
        m_cellGraph.addEntities(m_entityMgr->getEntityFromID(i));
    }

    std::cout<<"policeman 0 current cell = "<<m_police[0]->getCurrentCell()<<std::endl;



    //0.

    //(WHEN MAKING CELLS THEY NEED TO HAVE VECTORS OF ALL STATIC ENTITIES (walls n shit))

/// switchy magic:

//    switch (EntityMgr->getEntityFromID(i)->getEntityType())
//    {
//    case typePolice:


//        std::cout<<"Chub Police"<<std::endl;
//        break;

//    case typeRioter:

//        std::cout<<"Chub Rioter"<<std::endl;
//        break;

//    case typeWall:
//        std::cout<<"Chub Wall"<<std::endl;
//        break;

//    default:
//        std::cout<<"Who knows?"<<std::endl;
//        break;
//    }

/// end of switchy magic

    //1. for each agent -> updateCells
    // finds the cell agent is in; adds agentID to cell and cellID to agent.

    //2. for each moving entity (csGas, molotov etc.) -> updateCells
    // finds the cell moving entity is in; adds ID to cell and cellID to movingEntity.

    //3. for each agent -> get Neighbouring everything (other agents, moving entities, walls, obstacles)
    // get all the IDs from current and all neighbouring cells; check if within detectionRadius
    // add to one vector of IDs? seperate vectors? when to we distinguish? Cell level? ahhhhhhhhhh

    //4. same loop? update agent.

/// ---------------------------------------------------------------------------------



    //Update all the cells with their new vector of agents and agents with current Cell
//    for(int i = 0; i<m_rioters.size();i++)
//    {
//        //Give cellGraph the position of the agent, return the cell Id
//        //and append agent to that cell's vector of agents.
//        m_cellGraph.updateCells(m_rioters[i]);
//    }

    //Now we have updated cell information, return the neighbours of each agent:
//    for (int j = 0; j<m_rioters.size();j++)
//    {
//        m_cellGraph.addEntities(m_rioters[j]);
//    }





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

void GameWorld::loadMatricesToShader(ngl::Camera *cam, ngl::Mat4 mouseGlobalTX)
{

  ngl::Material m(ngl::Colour(0.2f,0.2f,0.2f, 1.0), ngl::Colour(0.1f,0.5f,0.1f, 1.0), ngl::Colour(0.77391f,0.77391f,0.77391f, 1.0));
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

