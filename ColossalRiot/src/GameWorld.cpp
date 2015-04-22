#include "GameWorld.h"

#include <iostream>




GameWorld::GameWorld()
{



   m_cellGraph =  CellGraph("plane_Test.obj");



  for (int i = 0; i < 2; ++i)
  {
    Rioter* newRioter = new Rioter(this);
    newRioter->setPos(ngl::Vec3(0.1, 0.0, 0.0));
    m_rioters.push_back(newRioter);


  }
  for (int i = 0; i < 5; ++i)
  {
    Police* newPolice = new Police(this);
    //newPolice->setTargetID(i);
    newPolice->setPos(ngl::Vec3(3.0, 0.0, 5.0));
    m_police.push_back(newPolice);
  }





//  Police* policeman = m_police[0];
//  Rioter* riotman = m_rioters[0];
//  policeman->Steering()->PursuitOn();
//  policeman->Steering()->setTargetAgent(riotman);
  //riotman->Steering()->setTargetAgent(policeman);
  //riotman->Steering()->EvadeOn();
//  riotman->Steering()->WanderOn();



}
void GameWorld::Update(double timeElapsed, double currentTime)
{



    //Clear the cells of agentIDs

    m_cellGraph.clearCells();

/// ORDER OF UPDATE:

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



    //Update all the cells with their new vector of agents and agents with current Cell
    for(int i = 0; i<m_rioters.size();i++)
    {
        //Give cellGraph the position of the agent, return the cell Id
        //and append agent to that cell's vector of agents.
        m_cellGraph.updateCells(m_rioters[i]);
    }

    //Now we have updated cell information, return the neighbours of each agent:
    for (int j = 0; j<m_rioters.size();j++)
    {
        m_cellGraph.addEntities(m_rioters[j]);
    }





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
