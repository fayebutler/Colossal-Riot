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

        Cell newCell(j,fourCorners,neighbourIDs);
        m_cells.push_back(newCell);

        if (j == 0)
        {
            m_cellSize = m_cells[0].getSize();
            //std::cout<<"cell size = "<<m_cellSize<<std::endl;
        }

    }
    m_maxDist = sqrt(2*((m_cellSize/2)*(m_cellSize/2)));
    //std::cout<<"maxDist = "<<m_maxDist<<std::endl;


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


    //std::cout<< "************************Current Entity:  "<< _entity->getID()<<std::endl;
    //for each neighbour

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
            //std::cout<<" Entity: "<< _entity->getID()<< " tests against entity: "<< dynamicEntitiesInCell[i]<< std::endl;

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
        //std::cout<<" Entity: "<< _entity->getID()<< " tests against entity: "<< dynamicEntitiesInCell[i]<< std::endl;

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




}
