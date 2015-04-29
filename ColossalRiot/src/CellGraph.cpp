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

    for (unsigned int j = 0; j < faces.size(); j++)
    {
        std::vector<ngl::Vec3> fourCorners;

        fourCorners.push_back(vertices[(faces[j].m_x-1)]);
        fourCorners.push_back(vertices[(faces[j].m_y-1)]);
        fourCorners.push_back(vertices[(faces[j].m_z-1)]);
        fourCorners.push_back(vertices[(faces[j].m_w-1)]);


       //FIND NEIGHBOURS

       std::vector<int> neighbourIDs;
       std::vector<int> perpendicularNeighbourIDs;

        for (unsigned int i=0 ;i<faces.size();i++)
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
            std::cout<<"cell size = "<<m_cellSize<<std::endl;
        }

    }
    m_maxDist = sqrt(2*((m_cellSize/2)*(m_cellSize/2)));
    std::cout<<"maxDist = "<<m_maxDist<<std::endl;

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



}

CellGraph::CellGraph()
{
  //m_entityMgr = new EntityManager();
}

void CellGraph::printCellGraph()
{

    for (unsigned int j =0; j<m_cells.size();j++)
    {
        m_cells[j].printCellInfo();
    }
}

void CellGraph::updateCells(BaseGameEntity *_entity)
{

//    ngl::Vec3 agentPos = _agent->getPos();
    for (unsigned int i=0; i < m_cells.size(); i++)
    {
        ngl::Vec3 test =m_cells[i].getCentre()-_entity->getPos();
        //std::cout<<" CurrentID:  "<< i<<std::endl;
        if (test.length()< m_maxDist)
        {
//            std::cout<<" CurrentID:  "<< i<<std::endl;
            m_cells[i].addDynamicEntityID(_entity->getID());
            _entity->setCurrentCellID(i);

            break;
        }

    }

}

void CellGraph::clearCells()
{

    for (unsigned int i = 0; i<m_cells.size(); i++)
    {
        m_cells[i].clearDynamicEntityIDs();
    }
}

void CellGraph::addEntities(BaseGameEntity *_entity)
{
// 1. add agents in current cell
// 2. add agents in neighbouring cells

    //Clear the ID vectors of the previous tick:
    _entity->clearDetectedDynamicEntityID();
    _entity->clearAgentIDs();

//    std::cout<< "************************Current Entity:  "<< _entity->getID()<<std::endl;
//    //for each neighbour
    int numberOfCellsToCheck = m_cells[(_entity->getCurrentCell())].getNeighbourCellIDs().size();

 //loops through all neighbour cells
    for (int i = 0; i < numberOfCellsToCheck; i++)
    {


        int currentNeighbourCell =m_cells[(_entity->getCurrentCell())].getNeighbourCellIDs()[i];
        std::vector<int> dynamicEntitiesInCell = m_cells[currentNeighbourCell].getDynamicEntityIDs();
//        std::cout<<"Number of Neighbours :  "<< dynamicEntitiesInCell.size()<<" in cell "<<m_cells[(_entity->getCurrentCell())].getNeighbourCellIDs()[i]<<std::endl;


        //Tests all entities in neighbouring cells against the entity's detection radius and adds it to its detected neighbours:
        for (unsigned int i = 0; i < dynamicEntitiesInCell.size(); i++)
        {
            std::cout<<" Entity: "<< _entity->getID()<< " tests against entity: "<< dynamicEntitiesInCell[i]<< std::endl;

            ngl::Vec3 dist  = (m_entityMgr->getEntityFromID(dynamicEntitiesInCell[i])->getPos()-(_entity->getPos()));
            if(dist.length()<_entity->getDetectionRadius())
            {
                //All entities will have a list of neighbours within their radius'
                _entity->addDetectedDynamicEntityID(dynamicEntitiesInCell[i]);

//                std::cout<<"***********************Total detected entities:  "<<_entity->getDetectedEntityIDs().size()<<std::endl;
            }

        }


    }

    //Do the same test but against the cell the entity's in:
    std::vector<int> dynamicEntitiesInCell = m_cells[_entity->getCurrentCell()].getDynamicEntityIDs();
//    std::cout<<"Number of Neighbours :  "<< dynamicEntitiesInCell.size()<<" in cell "<<_entity->getCurrentCell()<<std::endl;
    for (unsigned int i = 0; i < dynamicEntitiesInCell.size(); i++)
    {
        if (_entity->getID() != dynamicEntitiesInCell[i])
        {
        std::cout<<" Entity: "<< _entity->getID()<< " tests against entity: "<< dynamicEntitiesInCell[i]<< std::endl;

        ngl::Vec3 dist  = (m_entityMgr->getEntityFromID(dynamicEntitiesInCell[i])->getPos()-(_entity->getPos()));
        if(dist.length()<_entity->getDetectionRadius())
        {
            //All entities will have a list of neighbours within their radius'
            _entity->addDetectedDynamicEntityID(dynamicEntitiesInCell[i]);

//            std::cout<<"***********************Total detected entities:  "<<_entity->getDetectedEntityIDs().size()<<std::endl;
        }

        }
    }


    //Now the entity has a vector of detected entities, append them to the appropriate vector ie Police, Rioters etc
    for (unsigned int i =0; i< _entity->getDetectedEntityIDs().size(); i++)
    {
//        std::cout<<"WOOOOOO       "<<_entity->getDetectedEntityIDs()[i]<<std::endl;

        if ( m_entityMgr->getEntityFromID(_entity->getDetectedEntityIDs()[i])->getEntityType() == typePolice)
        {
//            std::cout<<"WOOOOOO     POPO  "<<_entity->getDetectedEntityIDs()[i]<<std::endl;

            _entity->addPoliceID(_entity->getDetectedEntityIDs()[i]);
        }
        if ( m_entityMgr->getEntityFromID(_entity->getDetectedEntityIDs()[i])->getEntityType() == typeRioter)
        {
//            std::cout<<"WOOOOOO   RIOT    "<<_entity->getDetectedEntityIDs()[i]<<std::endl;

            _entity->addRioterID(_entity->getDetectedEntityIDs()[i]);
        }
    }



}

void CellGraph::generateWalls()
{
    for (int i=0; i< m_cells.size(); i++)
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

}

//TO D000 :Instead of cells as inputs, use Vec3's
std::vector<ngl::Vec3> CellGraph::findPath(Cell _from, Cell _to)
{



   int currentCell;
   std::vector<int> frontierCells; //IDs of cells to be tested
   std::vector<int> frontierMemory; // list of all cell IDs that have ever been held in frontierCells.

   std::vector<int> priorityQueue;

   std::vector<std::vector<int> > SPTs; // vector of search paths (of cell IDs), new ones are added when cells cannot be added sequentially.
                                        // This happens when a new frontier is tested that is not perpendicular to the previous SPT element.



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

    ///4-Go to highes priority cell:
    // Set currentCell to the first element of priorityQueue.

    ///5- Idf currentCell == _to (destination).


//    //3-Then,  add all perpendicular neighbours of that cell to the frontier cell vector if not in frontierMemory.

//    //Initially, set frontier to all neighbouring cells:
//    for( int i =0; i<_from.getPerpendicularNeighbourCellIDs().size(); i++)
//    {
//        frontierCells.push_back(&m_cells[_from.getPerpendicularNeighbourCellIDs()[i]]);
//    }

//    for ( int i=0; i< frontierCells.size(); i++)
//    {
//        //Cost in our system is the distance from the start cell to the frontier tested cell
//        float newCost = (frontierCells[i]->getCentre() - _from.getCentre()).length();

//        //If the cell has not been
//        if( frontierCells[i])


//    }



}
