#include "CellGraph.h"


//reads in obj with uniform quads, makes a cell from each face. Cells are used for cell space partitioning and path finding. Obj should be the walkable area of the map.
CellGraph::CellGraph(const char *_fileName, int _borderSize)
{

    m_entityMgr = new EntityManager();

    std::vector<ngl::Vec3> vertices;
    std::vector<ngl::Vec4> faces;
    float upperBoundry = 0.0f;
    float lowerBoundry = 0.0f;
    float leftBoundry = 0.0f;
    float rightBoundry = 0.0f;

    std::ifstream in(_fileName, std::ios::in);
    if (!in) { std::cerr<< "Cannot open file: " << _fileName << std::endl; exit(1); }

    std::string line;
    while (getline(in, line))
    {
        //make a vector of vertices
        if (line.substr(0,2) == "v ")
        {
          std::istringstream s(line.substr(2));
          ngl::Vec3 v; s >> v.m_x; s >> v.m_y; s >> v.m_z;
          vertices.push_back(v);
          if(v.m_z < upperBoundry){upperBoundry = v.m_z;}
          if(v.m_z > lowerBoundry){lowerBoundry = v.m_z;}
          if(v.m_x < leftBoundry){leftBoundry = v.m_x;}
          if(v.m_x > rightBoundry){rightBoundry = v.m_x;}
        }

        //make a vector of faces
        else if (line.substr(0,2) == "f ")
        {
           std::istringstream s(line.substr(2));
           std::string rawVertId1, rawVertId2,rawVertId3,rawVertId4;

           s >> rawVertId1;
           s >> rawVertId2;
           s >> rawVertId3;
           s >> rawVertId4;

           std::string vertId1,vertId2,vertId3,vertId4;

           //Get rid of slashes in .obj files:
           for(unsigned int i =0; i<rawVertId1.length(); i++)
           {
             if (std::ispunct(rawVertId1[i]))
             {
                 break;
             }
             vertId1 = vertId1+rawVertId1[i];
           }
           for(unsigned int i =0; i<rawVertId2.length(); i++)
           {
             if (std::ispunct(rawVertId2[i]))
             {
                 break;
             }
             vertId2 = vertId2+rawVertId2[i];
           }


           for(unsigned int i =0; i<rawVertId3.length(); i++)
           {
             if (std::ispunct(rawVertId3[i]))
             {
                 break;
             }
             vertId3 = vertId3+rawVertId3[i];
           }


           for(unsigned int i =0; i<rawVertId4.length(); i++)
           {
             if (std::ispunct(rawVertId4[i]))
             {
                 break;
             }
             vertId4 = vertId4+rawVertId4[i];
           }

           int a = std::atoi(vertId1.c_str());
           int b =std::atoi(vertId2.c_str());
           int c =std::atoi(vertId3.c_str());
           int d = std::atoi(vertId4.c_str());

           ngl::Vec4 newFace;
           newFace.m_x=a;
           newFace.m_y=b;
           newFace.m_z=c;
           newFace.m_w=d;

           faces.push_back(newFace);
        }
        else if (line[0] == '#') { /* ignoring this line */ }
        else { /* ignoring this line */ }
    }

    //Now we make the cells from the informations stored in faces.
    m_numberOfCells = faces.size();
    for (unsigned int j = 0; j < m_numberOfCells; j++)
    {
        std::vector<ngl::Vec3> fourCorners;

        fourCorners.push_back(vertices[(faces[j].m_x-1)]);
        fourCorners.push_back(vertices[(faces[j].m_y-1)]);
        fourCorners.push_back(vertices[(faces[j].m_z-1)]);
        fourCorners.push_back(vertices[(faces[j].m_w-1)]);


       //find the neighbouring cells (cells that share vertices)

       std::vector<int> neighbourIDs;
       std::vector<int> perpendicularNeighbourIDs;

        for (unsigned int i=0 ;i<m_numberOfCells;i++)
        {
            // j is the cell (face) we are creating. i is cell we are checking against.
           if (faces[j].m_x == faces[i].m_x || faces[j].m_y == faces[i].m_x ||
                   faces[j].m_z == faces[i].m_x || faces[j].m_w == faces[i].m_x||

                   faces[j].m_x == faces[i].m_y || faces[j].m_y == faces[i].m_y ||
                                       faces[j].m_z == faces[i].m_y || faces[j].m_w == faces[i].m_y||

                   faces[j].m_x == faces[i].m_z || faces[j].m_y == faces[i].m_z ||
                                       faces[j].m_z == faces[i].m_z || faces[j].m_w == faces[i].m_z||

                   faces[j].m_x == faces[i].m_w || faces[j].m_y == faces[i].m_w ||
                                       faces[j].m_z == faces[i].m_w || faces[j].m_w == faces[i].m_w)
           {
               if( i != j)
               {


               neighbourIDs.push_back(i);
               }
           }
        }

        Cell newCell(j,fourCorners,neighbourIDs, perpendicularNeighbourIDs);
        m_cells.push_back(newCell);

        if (j == 0)
        {
            m_cellSize = m_cells[0].getSize();
        }

    }

    //Give each cell a list of its perpendicular nieghbours
    for ( int i =0; i< m_cells.size(); i++)
    {
        for ( int j=0; j< m_cells[i].getNeighbourCellIDs().size();j++)
        {
            m_cells[m_cells[i].getNeighbourCellIDs()[j]];
            if ( m_cells[m_cells[i].getNeighbourCellIDs()[j]].getCentre().m_x == m_cells[i].getCentre().m_x
                 || m_cells[m_cells[i].getNeighbourCellIDs()[j]].getCentre().m_z == m_cells[i].getCentre().m_z)
            {
                m_cells[i].addPerpendicularNeighbourID(m_cells[i].getNeighbourCellIDs()[j]);
            }
        }
    }

    //now we offset the boundries to define playable area
    m_mapBounds.push_back(upperBoundry+m_cellSize*_borderSize);
    m_mapBounds.push_back(lowerBoundry-m_cellSize*_borderSize);
    m_mapBounds.push_back(leftBoundry+m_cellSize*_borderSize);
    m_mapBounds.push_back(rightBoundry-m_cellSize*_borderSize);
}

CellGraph::CellGraph()
{
  //m_entityMgr = new EntityManager();
}

CellGraph::~CellGraph()
{
  delete m_entityMgr;
}

void CellGraph::printCellGraph()
{

    for (unsigned int j =0; j< m_numberOfCells;j++)
    {
        m_cells[j].printCellInfo();
    }
}

bool CellGraph::entityIsInCell(int _cellID, BaseGameEntity *_entity)
{
  //check up down left right

  float upper = m_cells[_cellID].getBoundaries().m_x;
  float lower = m_cells[_cellID].getBoundaries().m_y;
  float left  = m_cells[_cellID].getBoundaries().m_z;
  float right = m_cells[_cellID].getBoundaries().m_w;

  if(_entity->getPos().m_z > upper && _entity->getPos().m_z < lower &&
     _entity->getPos().m_x > left && _entity->getPos().m_x < right)
  {
    return true;
  }

  else
  {
   return false;
  }

}

void CellGraph::initializeCells(BaseGameEntity *_entity)
{
      for (unsigned int i=0; i < m_numberOfCells; i++)
      {

          if (entityIsInCell(i, _entity) == true)
          {
              m_cells[i].addDynamicEntityID(_entity->getID());
              _entity->setCurrentCellID(i);
              _entity->setCurrentCell(m_cells[i]);
              return;
          }
      }
      _entity->setCurrentCellID(-1);
}




void CellGraph::updateCells(BaseGameEntity *_entity)
{
  if (entityIsInCell(_entity->getCurrentCellID(), _entity) == true)
  {
    m_cells[_entity->getCurrentCellID()].addDynamicEntityID(_entity->getID());
  }
  else
  {
    for (int i = 0; i < _entity->getCurrentCell().getNeighbourCellIDs().size(); i++)
    {
      int currentNeighbourID = _entity->getCurrentCell().getNeighbourCellIDs()[i];
      if (entityIsInCell(currentNeighbourID, _entity) == true)
      {
        m_cells[currentNeighbourID].addDynamicEntityID(_entity->getID());
        _entity->setCurrentCellID(currentNeighbourID);
        _entity->setCurrentCell(m_cells[currentNeighbourID]);
      }
    }
  }
}

void CellGraph::clearCells()
{

    for (unsigned int i = 0; i<m_numberOfCells; i++)
    {
        m_cells[i].clearDynamicEntityIDs();
    }
}

void CellGraph::addEntities(BaseGameEntity *_entity)
{
  // 1. add agents in current cell
  // 2. add agents in neighbouring cells

  if (_entity >= 0)
  {

    //Clear the ID vectors of the previous tick:
    _entity->clearDetectedDynamicEntityID();
    _entity->clearAgentIDs();

    //Do the test but against the cell the entity's in:
   int numberOfEntiesInCurrentCell = m_cells[_entity->getCurrentCellID()].getDynamicEntityIDs().size();


   for (unsigned int i = 0; i < numberOfEntiesInCurrentCell; i++)
   {
       if (_entity->getID() != m_cells[_entity->getCurrentCellID()].getDynamicEntityIDs()[i])
       {


          _entity->addDetectedDynamicEntityID(m_cells[_entity->getCurrentCellID()].getDynamicEntityIDs()[i]);
       }

   }

    //for each neighbour
    int numberOfCellsToCheck = m_cells[(_entity->getCurrentCellID())].getNeighbourCellIDs().size();
    //loops through all neighbour cells
    for (int i = 0; i < numberOfCellsToCheck; i++)
    {

        int currentNeighbourCell = m_cells[(_entity->getCurrentCellID())].getNeighbourCellIDs()[i];
        //Tests all entities in neighbouring cells against the entity's detection radius and adds it to its detected neighbours:
        numberOfEntiesInCurrentCell = m_cells[currentNeighbourCell].getDynamicEntityIDs().size();

        for (unsigned int i = 0; i < numberOfEntiesInCurrentCell; i++)
        {
            //std::cout<<"called by addEntities"<<std::endl;

          ngl::Vec3 vectorToEntity = _entity->getPos()- (m_entityMgr->getEntityFromID(m_cells[currentNeighbourCell].getDynamicEntityIDs()[i])->getPos());
          if(vectorToEntity.lengthSquared() <= (_entity->getDetectionRadius()*_entity->getDetectionRadius()))
          {
            _entity->addDetectedDynamicEntityID(m_cells[currentNeighbourCell].getDynamicEntityIDs()[i]);

          }
        }
    }

  }
  //Now the entity has a vector of detected entities, append them to the appropriate vector ie Police, Rioters etc
  for (unsigned int i =0; i< _entity->getDetectedEntityIDs().size(); i++)
  {
      //std::cout<<"called by add Entities 2"<<std::endl;
      if ( m_entityMgr->getEntityFromID(_entity->getDetectedEntityIDs()[i])->getEntityType() == typePolice)
      {
          _entity->addPoliceID(_entity->getDetectedEntityIDs()[i]);
      }
      //std::cout<<"called by addEntities 3"<<std::endl;
      if ( m_entityMgr->getEntityFromID(_entity->getDetectedEntityIDs()[i])->getEntityType() == typeRioter)
      {
          _entity->addRioterID(_entity->getDetectedEntityIDs()[i]);
      }
  }
}

void CellGraph::generateWalls()
{
    for (unsigned int i=0; i< m_numberOfCells; i++)
    {
        bool upperWall = true, lowerWall = true, leftWall =  true,  rightWall= true;

        //Test for each wall against 3 conditions,
        //if no neighbour centre is detected, the bool remains true:

        for ( int j =0; j < m_cells[i].getPerpendicularNeighbourCellIDs().size(); j ++)
        {
            ngl::Vec3 currentNeighbourCentre = m_cells[m_cells[i].getPerpendicularNeighbourCellIDs()[j]].getCentre();


            //Check for upper wall:
            if(currentNeighbourCentre.m_z < m_cells[i].getBoundaries().m_x
                    && currentNeighbourCentre.m_x > m_cells[i].getBoundaries().m_z
                    && currentNeighbourCentre.m_x < m_cells[i].getBoundaries().m_w)
            {
                upperWall = false;
            }
            //Check for lower wall:
            if(currentNeighbourCentre.m_z > m_cells[i].getBoundaries().m_y
                    && currentNeighbourCentre.m_x > m_cells[i].getBoundaries().m_z
                    && currentNeighbourCentre.m_x < m_cells[i].getBoundaries().m_w)
            {
                lowerWall = false;
            }
            //Check for left wall:
            if(currentNeighbourCentre.m_x < m_cells[i].getBoundaries().m_z
                    && currentNeighbourCentre.m_z < m_cells[i].getBoundaries().m_y
                    && currentNeighbourCentre.m_z > m_cells[i].getBoundaries().m_x)
            {
                leftWall = false;
            }
            //Check for right wall
            if(currentNeighbourCentre.m_x > m_cells[i].getBoundaries().m_w
                    && currentNeighbourCentre.m_z < m_cells[i].getBoundaries().m_y
                    && currentNeighbourCentre.m_z > m_cells[i].getBoundaries().m_x)
            {
                rightWall = false;
            }



        }

        //Now make the walls depending on the bool
        if(upperWall == true)
        {
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_x);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_x);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(0.0f,0.0f,1.0f);
            m_cells[i].addWallInCell(newWall);

        }
        if(lowerWall == true)
        {
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_y);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_y);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(0.0f,0.0f,-1.0f);
            m_cells[i].addWallInCell(newWall);

        }
        if(leftWall == true)
        {
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_y);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_x);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(1.0f,0.0f,0.0f);
            m_cells[i].addWallInCell(newWall);

        }
        if(rightWall == true)
        {
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_x);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_y);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(-1.0f,0.0f,0.0f);
            m_cells[i].addWallInCell(newWall);
        }


    }
    for (int i =0; i < m_numberOfCells; i++)
        {
          for (int j = 0; j < m_cells[i].getWallsInCell().size(); j++)
          {
                Wall wallToAdd = m_cells[i].getWallsInCell()[j];
                m_cells[i].addWall(wallToAdd);
          }
            int numberOfNeighbours = m_cells[i].getNeighbourCellIDs().size();

            for(int j = 0; j < numberOfNeighbours; j++)
            {
                for( int k=0; k<m_cells[m_cells[i].getNeighbourCellIDs()[j]].getWallsInCell().size();k++)
                {
                    Wall wallToAdd = m_cells[m_cells[i].getNeighbourCellIDs()[j]].getWallsInCell()[k];
                    m_cells[i].addWall(wallToAdd);
                }

            }
        }

}



std::vector<ngl::Vec3> CellGraph::findPath(BaseGameEntity *_from, ngl::Vec3 _to)
{

    std::vector<ngl::Vec3> finalPath;
    finalPath.clear();

    int endCellID = -1;

    for (int i=0; i<m_numberOfCells; i++)
    {
        float upper = m_cells[i].getBoundaries().m_x;
        float lower = m_cells[i].getBoundaries().m_y;
        float left  = m_cells[i].getBoundaries().m_z;
        float right = m_cells[i].getBoundaries().m_w;

        if(_to.m_z >= upper && _to.m_z <= lower &&
           _to.m_x >= left && _to.m_x <= right)
        {
             endCellID = i;
             break;

        }

    }
    if(endCellID == -1)
    {
        std::cout<<"You have not chosen a valid position"<<std::endl;
        return finalPath;
    }

   int startCellID = _from->getCurrentCellID();
   Cell *endCell = &m_cells[endCellID];
   int currentCellID = startCellID;
   Cell *currentCell = &m_cells[currentCellID];



   std::vector<int> frontierCells; //IDs of cells to be tested
   std::vector<int> frontierMemory; // list of all cell IDs that have ever been held in frontierCells.

   std::vector<int> priorityQueue;

   std::vector<std::vector<int> > SPTs; // vector of search paths (of cell IDs), new ones are added when cells cannot be added sequentially.
                                        // This happens when a new frontier is tested that is not perpendicular to the previous SPT element.

   //Create start SPT, the firt element of which is  _from
   std::vector<int> newSPT;
   //Push it into SPTs
   newSPT.push_back(startCellID);
   SPTs.push_back(newSPT);

   frontierMemory.push_back(startCellID);

///////////////////////////////LOOOOOP////////////////////////////////////////////////////////////////////////////////////

    while (currentCellID != endCellID)
    {

///////1-Update frontier:

    //Clear priority Q:
    priorityQueue.clear();

    //Remove currentCell from frontierCells:
    for (std::vector<int>::iterator iter = frontierCells.begin(); iter != frontierCells.end(); ++iter)
    {
        if( *iter == currentCellID)
        {
            frontierCells.erase(iter);
            break;
        }
    }


    //Add perpendicular neighbours to frontierCells:
    int numberOfPerpendicularNeighbours = currentCell->getPerpendicularNeighbourCellIDs().size();
    for ( int i=0; i <numberOfPerpendicularNeighbours;i++)
    {
        bool isInMemory = false;

        for (int j=0; j<frontierMemory.size();j++)
        {
            if( currentCell->getPerpendicularNeighbourCellIDs()[i] == frontierMemory[j])
            {
                isInMemory = true;
                break;
            }


        }

        if ( isInMemory ==false)
        {

            frontierCells.push_back(currentCell->getPerpendicularNeighbourCellIDs()[i]);
            frontierMemory.push_back(currentCell->getPerpendicularNeighbourCellIDs()[i]);
        }

    }


//////2-Update priorityQueue:
    //Order frontier cells:
    std::vector<int>frontierCopy = frontierCells;
    int frontierSize = frontierCells.size();

    while (priorityQueue.size() < frontierSize)
    {
        float shortestDist = 10000000000.0f;
        int shortestID;

        for (int i=0; i<frontierCopy.size();i++ )
        {
            ngl::Vec3 distance = (m_cells[frontierCopy[i]].getCentre() - endCell->getCentre());

            if(distance.lengthSquared() <shortestDist)
            {
                shortestDist = distance.lengthSquared();
                shortestID = frontierCopy[i];
            }
        }

        for (std::vector<int>::iterator iter = frontierCopy.begin(); iter != frontierCopy.end(); ++iter)
        {
            if( *iter == shortestID)
            {
                frontierCopy.erase(iter);
                break;
            }
        }
        priorityQueue.push_back(shortestID);
    }

//    std::cout<<"Frontier Cells"<< std::endl;
    for (int i = 0;i<frontierCells.size();i++)
    {
//        std::cout<<frontierCells[i]<<std::endl;

    }

//    std::cout<< "PRIORITY QUEUEUE:   "<<std::endl;
    for (int i =0; i< priorityQueue.size();i++)
    {
//        std::cout<< priorityQueue[i]<<std::endl;
    }

//    std::cout<<"Frontier Memory: "<< std::endl;
    for (int i = 0;i<frontierMemory.size();i++)
    {
//        std::cout<<frontierMemory[i]<<std::endl;

    }


//3-Add currentCell to SPT
  //Check the latest value of the latest SPT, if the currentCell is perpendicular then add to that SPT.

bool flag = false;

if (currentCellID != startCellID)
{


    for (int i = SPTs.size()-1; i >= 0; i--)
    {
        newSPT.clear();

        for ( int j = 0; j < SPTs[i].size(); j++)
        {
            Cell *SPTCell = &m_cells[SPTs[i][j]];

            newSPT.push_back(SPTs[i][j]);

            for ( int k=0; k<SPTCell->getPerpendicularNeighbourCellIDs().size();k++)
            {
                if(flag == false)
                {
                    if(currentCellID == SPTCell->getPerpendicularNeighbourCellIDs()[k])
                    {
                        if(j+1==SPTs[i].size())
                        {
                            newSPT.push_back(currentCellID);

                            if(i == SPTs.size())
                            {
                                SPTs.back() = newSPT;
                            }
                            else
                            {
                                SPTs.push_back(newSPT);
                            }
                            flag = true;


                        }
                        else
                        {
                            newSPT.push_back(currentCellID);
                            SPTs.push_back(newSPT);
                            flag = true;
                        }
                    }
                }
            }
        }
    }
}


    //Now actual search:

    ////1-Update frontier:
    // Clear priorityQueue.
    // Remove currentCell from frontier.
    // Get perpendicular neighbours of cell. Add them to frontier if they are not in frontier memory.

    ///2-Update priorityQueue:
    // Order the frontier by closest to destination, in priorityQueue.

    ///3-Add current cell to search path tree:
    // Checks the last SPT in SPTs. From the first element, check if the currentCell is perpendicular.
    // If it is perpendicular to the final element, simply append to that SPT vector.
    // If not, then create a new SPT vector from the start of the current SPT to the first perpendicular element, append currentCell.
    // If not perpendicular to ANY elements in the latest SPT, go to the previous SPT and perform check again. -Do not delete the last SPT.

    ///4-Go to highest priority cell:
    // Set currentCell to the first element of priorityQueue.

    ///5- If currentCell == _to (destination). while loop


    currentCellID = priorityQueue[0];
    currentCell = &m_cells[currentCellID];

    }

///////////////////////////////////////////LOOP/////////////////////////////////////////////////////////////////////

    SPTs.back().push_back(endCellID);


    for( int i = 0; i< SPTs.back().size(); i++)
    {
      finalPath.push_back(m_cells[SPTs.back()[i]].getCentre());
    }

    finalPath.push_back(_to);

    return finalPath;
}
