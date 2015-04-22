#include "CellGraph.h"

CellGraph::CellGraph(const char *_fileName)
{
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
           for(int i =0; i<rawVertId1.length(); i++)
           {
             if (std::ispunct(rawVertId1[i]))
             {
                 break;
             }
             vertId1 = vertId1+rawVertId1[i];
           }
           for(int i =0; i<rawVertId2.length(); i++)
           {
             if (std::ispunct(rawVertId2[i]))
             {
                 break;
             }
             vertId2 = vertId2+rawVertId2[i];
           }


           for(int i =0; i<rawVertId3.length(); i++)
           {
             if (std::ispunct(rawVertId3[i]))
             {
                 break;
             }
             vertId3 = vertId3+rawVertId3[i];
           }


           for(int i =0; i<rawVertId4.length(); i++)
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

    for (int j = 0; j < faces.size(); j++)
    {
        std::vector<ngl::Vec3> fourCorners;

        fourCorners.push_back(vertices[(faces[j].m_x-1)]);
        fourCorners.push_back(vertices[(faces[j].m_y-1)]);
        fourCorners.push_back(vertices[(faces[j].m_z-1)]);
        fourCorners.push_back(vertices[(faces[j].m_w-1)]);


       //FIND NEIGHBOURS

       std::vector<int> neighbourIDs;

        for (int i=0 ;i<faces.size();i++)
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

}

void CellGraph::printCellGraph()
{

    for ( int j =0; j<m_cells.size();j++)
    {
        m_cells[j].printCellInfo();
    }
}

void CellGraph::updateCells(Agent *_agent)
{

//    ngl::Vec3 agentPos = _agent->getPos();
    for (int i=0; m_cells.size(); i++)
    {
        ngl::Vec3 test =m_cells[i].getCentre()-_agent->getPos();
        //std::cout<<" CurrentID:  "<< i<<std::endl;
        if (test.length()< m_maxDist)
        {
            std::cout<<" CurrentID:  "<< i<<std::endl;
            m_cells[i].addAgentID(_agent->getID());
            _agent->setCurrentCellID(i);

            break;
        }

    }

}

void CellGraph::clearCells()
{

    for (unsigned int i = 0; i<m_cells.size(); i++)
    {
        m_cells[i].printCellInfo();
        m_cells[i].clearAgentIDs();
    }
}

void CellGraph::addEntities(Agent *_agent)
{
// 1. add agents in current cell
// 2. add agents in neighbouring cells




    //for each neighbour
    int numberOfCellsToCheck = m_cells[(_agent->getCurrentCell())].getNeighbourCellIDs().size();


    for (unsigned int i = 0; i < numberOfCellsToCheck; i++)
    {
        std::cout<<"Current Neighbour:  "<<i<<std::endl;
        int currentNeighbourCell =m_cells[(_agent->getCurrentCell())].getNeighbourCellIDs()[i];
        std::vector<int> agentsInCell = m_cells[currentNeighbourCell].getAgentIDs();
        // FOR LOOP HERE FOR EACH AGENT IN GETAGENTidS,

        for (unsigned int i = 0; i < agentsInCell.size(); i++)
        {
            std::cout<<"balls";

        }


        //CHECK DISTANCE FROM CURRENT AGENT AND ADD TO LIST OF EITHER POLICE AND RIOTERS
    }
}

