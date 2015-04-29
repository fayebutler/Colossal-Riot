#include "CellGraph.h"

CellGraph::CellGraph(const char *_fileName)
{

   m_entityMgr = new EntityManager();

    std::vector<ngl::Vec3> vertices;
    std::vector<ngl::Vec4> faces;

    std::ifstream in(_fileName, std::ios::in);
    if (!in) { std::cerr << "Cannot open file: " << _fileName << std::endl; exit(1); }

    std::string line;
    while (getline(in, line))
    {
        if (line.substr(0,2) == "v ")
        {

          std::istringstream s(line.substr(2));
          ngl::Vec3 v; s >> v.m_x; s >> v.m_y; s >> v.m_z;
          vertices.push_back(v);
        }
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

///NOW ACTUALLY MAKE THE CELLS
    m_numberOfCells = faces.size();
    for (unsigned int j = 0; j < m_numberOfCells; j++)
    {
        std::vector<ngl::Vec3> fourCorners;

        fourCorners.push_back(vertices[(faces[j].m_x-1)]);
        fourCorners.push_back(vertices[(faces[j].m_y-1)]);
        fourCorners.push_back(vertices[(faces[j].m_z-1)]);
        fourCorners.push_back(vertices[(faces[j].m_w-1)]);


       //FIND NEIGHBOURS

       std::vector<int> neighbourIDs;

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

        Cell newCell(j,fourCorners,neighbourIDs);
        m_cells.push_back(newCell);

        if (j == 0)
        {
            m_cellSize = m_cells[0].getSize();
            std::cout<<"cell size = "<<m_cellSize<<std::endl;
        }

    }
    m_maxDist = sqrt(2*((m_cellSize/2)*(m_cellSize/2)));
    std::cout<<"maxDist = "<<m_maxDist<<std::endl;
}

CellGraph::CellGraph()
{
  //m_entityMgr = new EntityManager();
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
<<<<<<< .merge_file_TbP41j
      int currentNeighbourID = _entity->getCurrentCell().getNeighbourCellIDs()[i];
      if (entityIsInCell(currentNeighbourID, _entity) == true)
      {
        m_cells[currentNeighbourID].addDynamicEntityID(_entity->getID());
        _entity->setCurrentCellID(currentNeighbourID);
        _entity->setCurrentCell(m_cells[currentNeighbourID]);
      }
=======
        ngl::Vec3 test =m_cells[i].getCentre()-_entity->getPos();
        //std::cout<<" CurrentID:  "<< i<<std::endl;
        if (test.length()< m_maxDist)
        {
//            std::cout<<" CurrentID:  "<< i<<std::endl;
            m_cells[i].addDynamicEntityID(_entity->getID());
            _entity->setCurrentCellID(i);
>>>>>>> .merge_file_cNQttp


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

<<<<<<< .merge_file_TbP41j
    //Do the test but against the cell the entity's in:
   int numberOfEntiesInCurrentCell = m_cells[_entity->getCurrentCellID()].getDynamicEntityIDs().size();
=======
//    std::cout<< "************************Current Entity:  "<< _entity->getID()<<std::endl;
//    //for each neighbour
    int numberOfCellsToCheck = m_cells[(_entity->getCurrentCell())].getNeighbourCellIDs().size();
>>>>>>> .merge_file_cNQttp


   for (unsigned int i = 0; i < numberOfEntiesInCurrentCell; i++)
   {
       if (_entity->getID() != m_cells[_entity->getCurrentCellID()].getDynamicEntityIDs()[i])
       {
          _entity->addDetectedDynamicEntityID(m_cells[_entity->getCurrentCellID()].getDynamicEntityIDs()[i]);
       }

   }

<<<<<<< .merge_file_TbP41j
    //for each neighbour
    int numberOfCellsToCheck = m_cells[(_entity->getCurrentCellID())].getNeighbourCellIDs().size();
    //loops through all neighbour cells
=======
        int currentNeighbourCell =m_cells[(_entity->getCurrentCell())].getNeighbourCellIDs()[i];
        std::vector<int> dynamicEntitiesInCell = m_cells[currentNeighbourCell].getDynamicEntityIDs();
//        std::cout<<"Number of Neighbours :  "<< dynamicEntitiesInCell.size()<<" in cell "<<m_cells[(_entity->getCurrentCell())].getNeighbourCellIDs()[i]<<std::endl;
>>>>>>> .merge_file_cNQttp

    for (int i = 0; i < numberOfCellsToCheck; i++)
    {

        int currentNeighbourCell = m_cells[(_entity->getCurrentCellID())].getNeighbourCellIDs()[i];
        //Tests all entities in neighbouring cells against the entity's detection radius and adds it to its detected neighbours:
        numberOfEntiesInCurrentCell = m_cells[currentNeighbourCell].getDynamicEntityIDs().size();

        for (unsigned int i = 0; i < numberOfEntiesInCurrentCell; i++)
        {

<<<<<<< .merge_file_TbP41j
          ngl::Vec3 vectorToEntity = _entity->getPos()- (m_entityMgr->getEntityFromID(m_cells[currentNeighbourCell].getDynamicEntityIDs()[i])->getPos());
=======
//                std::cout<<"***********************Total detected entities:  "<<_entity->getDetectedEntityIDs().size()<<std::endl;
            }
>>>>>>> .merge_file_cNQttp


          if(vectorToEntity.lengthSquared()< (m_maxDist*m_maxDist))
          {
            _entity->addDetectedDynamicEntityID(m_cells[currentNeighbourCell].getDynamicEntityIDs()[i]);

          }
        }
    }

<<<<<<< .merge_file_TbP41j
  }
  //Now the entity has a vector of detected entities, append them to the appropriate vector ie Police, Rioters etc
  for (unsigned int i =0; i< _entity->getDetectedEntityIDs().size(); i++)
  {
      if ( m_entityMgr->getEntityFromID(_entity->getDetectedEntityIDs()[i])->getEntityType() == typePolice)
      {
          _entity->addPoliceID(_entity->getDetectedEntityIDs()[i]);
      }
      if ( m_entityMgr->getEntityFromID(_entity->getDetectedEntityIDs()[i])->getEntityType() == typeRioter)
      {
          _entity->addRioterID(_entity->getDetectedEntityIDs()[i]);
      }
  }
}

void CellGraph::generateWalls()
{
    for (unsigned int i=0; i< m_cells.size(); i++)
=======
    //Do the same test but against the cell the entity's in:
    std::vector<int> dynamicEntitiesInCell = m_cells[_entity->getCurrentCell()].getDynamicEntityIDs();
//    std::cout<<"Number of Neighbours :  "<< dynamicEntitiesInCell.size()<<" in cell "<<_entity->getCurrentCell()<<std::endl;
    for (unsigned int i = 0; i < dynamicEntitiesInCell.size(); i++)
>>>>>>> .merge_file_cNQttp
    {
        bool upperWall = true, lowerWall = true, leftWall =  true,  rightWall= true;

        //Test for each wall against 3 conditions,
        //if no neighbour centre is detected, the bool remains true:
        for (unsigned int j =0; j < m_cells[i].getNeighbourCellIDs().size(); j ++)
        {
            ngl::Vec3 currentNeighbourCentre = m_cells[m_cells[i].getNeighbourCellIDs()[j]].getCentre();

<<<<<<< .merge_file_TbP41j
=======
//            std::cout<<"***********************Total detected entities:  "<<_entity->getDetectedEntityIDs().size()<<std::endl;
        }
>>>>>>> .merge_file_cNQttp

            //Check for upper wall:
            if(currentNeighbourCentre.m_z < m_cells[i].getBoundaries().m_x
                    && currentNeighbourCentre.m_x > m_cells[i].getBoundaries().m_z
                    && currentNeighbourCentre.m_x < m_cells[i].getBoundaries().m_w)
            {
                upperWall = false;
                //std::cout<< "Cell: "<< i<< "   UpperWall? " << upperWall<<std::endl;
            }
            //Check for lower wall:
            if(currentNeighbourCentre.m_z > m_cells[i].getBoundaries().m_y
                    && currentNeighbourCentre.m_x > m_cells[i].getBoundaries().m_z
                    && currentNeighbourCentre.m_x < m_cells[i].getBoundaries().m_w)
            {
                lowerWall = false;
              //std::cout<< "Cell: "<< i<< "   LowerWall? " << lowerWall<<std::endl;
            }
            //Check for left wall:
            if(currentNeighbourCentre.m_x < m_cells[i].getBoundaries().m_z
                    && currentNeighbourCentre.m_z < m_cells[i].getBoundaries().m_y
                    && currentNeighbourCentre.m_z > m_cells[i].getBoundaries().m_x)
            {
                leftWall = false;
                //std::cout<< "Cell: "<< i<< "   LeftWall? " << leftWall<<std::endl;
            }
            //Check for right wall
            if(currentNeighbourCentre.m_x > m_cells[i].getBoundaries().m_w
                    && currentNeighbourCentre.m_z < m_cells[i].getBoundaries().m_y
                    && currentNeighbourCentre.m_z > m_cells[i].getBoundaries().m_x)
            {
                rightWall = false;
               // std::cout<< "Cell: "<< i<< "   rightWall? " << rightWall<<std::endl;
            }


<<<<<<< .merge_file_TbP41j
=======
    //Now the entity has a vector of detected entities, append them to the appropriate vector ie Police, Rioters etc
    for (unsigned int i =0; i< _entity->getDetectedEntityIDs().size(); i++)
    {
//        std::cout<<"WOOOOOO       "<<_entity->getDetectedEntityIDs()[i]<<std::endl;
>>>>>>> .merge_file_cNQttp

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
            //std::cout<<"cell "<<i<<" upperWallstart: "<<start.m_x<<" "<<start.m_z<<" upperwallend: "<<end.m_x<<" "<<end.m_z<<std::endl;
            m_cells[i].addWall(newWall);

        }
        if(lowerWall == true)
        {
<<<<<<< .merge_file_TbP41j
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_y);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_y);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(0.0f,0.0f,-1.0f);
            //std::cout<<"cell "<<i<<" lowerWallstart: "<<start.m_x<<" "<<start.m_z<<" lowerwallend: "<<end.m_x<<" "<<end.m_z<<std::endl;
            m_cells[i].addWall(newWall);
=======
//            std::cout<<"WOOOOOO     POPO  "<<_entity->getDetectedEntityIDs()[i]<<std::endl;
>>>>>>> .merge_file_cNQttp

        }
        if(leftWall == true)
        {
<<<<<<< .merge_file_TbP41j
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_y);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_x);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(1.0f,0.0f,0.0f);
            //std::cout<<"cell "<<i<<" leftWallstart: "<<start.m_x<<" "<<start.m_z<<" leftwallend: "<<end.m_x<<" "<<end.m_z<<std::endl;
            m_cells[i].addWall(newWall);
=======
//            std::cout<<"WOOOOOO   RIOT    "<<_entity->getDetectedEntityIDs()[i]<<std::endl;
>>>>>>> .merge_file_cNQttp

        }
        if(rightWall == true)
        {
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_x);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_y);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(-1.0f,0.0f,0.0f);
            //std::cout<<"cell "<<i<<" rightWallstart: "<<start.m_x<<" "<<start.m_z<<" rightwallend: "<<end.m_x<<" "<<end.m_z<<std::endl;
            m_cells[i].addWall(newWall);
        }


    }




<<<<<<< .merge_file_TbP41j
=======
void CellGraph::generateWalls()
{
    for (int i=0; i< m_cells.size(); i++)
    {
        bool upperWall = true, lowerWall = true, leftWall =  true,  rightWall= true;

        //Test for each wall against 3 conditions,
        //if no neighbour centre is detected, the bool remains true:
        for ( int j =0; j < m_cells[i].getNeighbourCellIDs().size(); j ++)
        {
            ngl::Vec3 currentNeighbourCentre = m_cells[m_cells[i].getNeighbourCellIDs()[j]].getCentre();


            //Check for upper wall:
            if(currentNeighbourCentre.m_z < m_cells[i].getBoundaries().m_x
                    && currentNeighbourCentre.m_x > m_cells[i].getBoundaries().m_z
                    && currentNeighbourCentre.m_x < m_cells[i].getBoundaries().m_w)
            {
                upperWall = false;
                //std::cout<< "Cell: "<< i<< "   UpperWall? " << upperWall<<std::endl;
            }
            //Check for lower wall:
            if(currentNeighbourCentre.m_z > m_cells[i].getBoundaries().m_y
                    && currentNeighbourCentre.m_x > m_cells[i].getBoundaries().m_z
                    && currentNeighbourCentre.m_x < m_cells[i].getBoundaries().m_w)
            {
                lowerWall = false;
              //std::cout<< "Cell: "<< i<< "   LowerWall? " << lowerWall<<std::endl;
            }
            //Check for left wall:
            if(currentNeighbourCentre.m_x < m_cells[i].getBoundaries().m_z
                    && currentNeighbourCentre.m_z < m_cells[i].getBoundaries().m_y
                    && currentNeighbourCentre.m_z > m_cells[i].getBoundaries().m_x)
            {
                leftWall = false;
                //std::cout<< "Cell: "<< i<< "   LeftWall? " << leftWall<<std::endl;
            }
            //Check for right wall
            if(currentNeighbourCentre.m_x > m_cells[i].getBoundaries().m_w
                    && currentNeighbourCentre.m_z < m_cells[i].getBoundaries().m_y
                    && currentNeighbourCentre.m_z > m_cells[i].getBoundaries().m_x)
            {
                rightWall = false;
               // std::cout<< "Cell: "<< i<< "   rightWall? " << rightWall<<std::endl;
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
            //std::cout<<"cell "<<i<<" upperWallstart: "<<start.m_x<<" "<<start.m_z<<" upperwallend: "<<end.m_x<<" "<<end.m_z<<std::endl;
            m_cells[i].addWall(newWall);

        }
        if(lowerWall == true)
        {
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_y);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_y);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(0.0f,0.0f,-1.0f);
            //std::cout<<"cell "<<i<<" lowerWallstart: "<<start.m_x<<" "<<start.m_z<<" lowerwallend: "<<end.m_x<<" "<<end.m_z<<std::endl;
            m_cells[i].addWall(newWall);

        }
        if(leftWall == true)
        {
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_x);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_y);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(1.0f,0.0f,0.0f);
           //std::cout<<"cell "<<i<<" leftWallstart: "<<start.m_x<<" "<<start.m_z<<" leftwallend: "<<end.m_x<<" "<<end.m_z<<std::endl;
            m_cells[i].addWall(newWall);

        }
        if(rightWall == true)
        {
            ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_x);
            ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_y);
            Wall newWall;
            newWall.start =start;
            newWall.end = end;
            newWall.normal = ngl::Vec3(-1.0f,0.0f,0.0f);
            //std::cout<<"cell "<<i<<" rightWallstart: "<<start.m_x<<" "<<start.m_z<<" rightwallend: "<<end.m_x<<" "<<end.m_z<<std::endl;
            m_cells[i].addWall(newWall);
        }


    }




>>>>>>> .merge_file_cNQttp
}
