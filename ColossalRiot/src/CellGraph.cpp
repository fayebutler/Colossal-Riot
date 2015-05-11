#include "CellGraph.h"
//----------------------------------------------------------------------------------------------------------------------
/// @file CellGraph.cpp
/// @brief This class constructs a graph of cells using an .obj file.
///  this graph is used as a nav mesh, for spatial partitioning and path planning.
//----------------------------------------------------------------------------------------------------------------------

CellGraph::CellGraph(const char *_fileName, int _borderSize)
{
  // create new  entity manager
  m_entityMgr = new EntityManager();


  //make containers for vertices and faces from .obj
  std::vector<ngl::Vec3> vertices;
  std::vector<ngl::Vec4> faces;
  // initialize graph boundries
  float upperBoundry = 0.0f;
  float lowerBoundry = 0.0f;
  float leftBoundry = 0.0f;
  float rightBoundry = 0.0f;

  // error if file cannot be opened
  std::ifstream in(_fileName, std::ios::in);
  if (!in) { std::cerr<< "Cannot open file: " << _fileName << std::endl; exit(1); }

  //this loop reads in the .obj
  std::string line;
  while (getline(in, line))
    {
      //push back vertices into vector
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

      //push back faces into vector
      else if (line.substr(0,2) == "f ")
      {

        // for each face, store vertex indicies
        std::istringstream s(line.substr(2));
        std::string rawVertId1, rawVertId2,rawVertId3,rawVertId4;

        s >> rawVertId1;
        s >> rawVertId2;
        s >> rawVertId3;
        s >> rawVertId4;

        std::string vertId1,vertId2,vertId3,vertId4;

        //ignore slashes in .obj file:
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

        //converts vertex indicies to ints
        int a = std::atoi(vertId1.c_str());
        int b =std::atoi(vertId2.c_str());
        int c =std::atoi(vertId3.c_str());
        int d = std::atoi(vertId4.c_str());

        //now make a face with vertex indicies

        //faces stored as a vec4 of indicies
        ngl::Vec4 newFace;
        newFace.m_x=a;
        newFace.m_y=b;
        newFace.m_z=c;
        newFace.m_w=d;

        // push back face into vector of faces
        faces.push_back(newFace);
      }
      else if (line[0] == '#') { /* ignoring this line */ }
      else { /* ignoring this line */ }
    }

    //Now we make the cells from the informations stored in faces.
    m_numberOfCells = faces.size();

    for (unsigned int j = 0; j < m_numberOfCells; j++)
    {

      // create a vector of four corners
      std::vector<ngl::Vec3> fourCorners;

      fourCorners.push_back(vertices[(faces[j].m_x-1)]);
      fourCorners.push_back(vertices[(faces[j].m_y-1)]);
      fourCorners.push_back(vertices[(faces[j].m_z-1)]);
      fourCorners.push_back(vertices[(faces[j].m_w-1)]);


      // create a vector of IDs for neighbours and perpendicular neighbours to give to each cell
      std::vector<int> neighbourIDs;
      std::vector<int> perpendicularNeighbourIDs;

      // find neighbouring cells from faces that share vertices

      for (unsigned int i=0 ;i<m_numberOfCells;i++)
      {

        // j is the cell (face) we are creating. i is cell we are checking against.
        if( i != j)
        {
          //if any of the vertices are the same, push back that face into neighbours
          if (faces[j].m_x == faces[i].m_x || faces[j].m_y == faces[i].m_x ||
                 faces[j].m_z == faces[i].m_x || faces[j].m_w == faces[i].m_x||

                 faces[j].m_x == faces[i].m_y || faces[j].m_y == faces[i].m_y ||
                                     faces[j].m_z == faces[i].m_y || faces[j].m_w == faces[i].m_y||

                 faces[j].m_x == faces[i].m_z || faces[j].m_y == faces[i].m_z ||
                                     faces[j].m_z == faces[i].m_z || faces[j].m_w == faces[i].m_z||

                 faces[j].m_x == faces[i].m_w || faces[j].m_y == faces[i].m_w ||
                                     faces[j].m_z == faces[i].m_w || faces[j].m_w == faces[i].m_w)
            {
             neighbourIDs.push_back(i);
            }
         }

      }

      Cell newCell(j,fourCorners,neighbourIDs);
      m_cells.push_back(newCell);

      if (j == 0)
      {
        //gets size of first cell
        m_cellSize = m_cells[0].getSize();
      }
    }

    //Give each cell a list of its perpendicular nieghbours

    //for all the cells in the graph:
    for ( int i =0; i< m_numberOfCells; i++)
    {
      //for all that cells neighbours:
      for ( int j=0; j< m_cells[i].getNeighbourCellIDs().size();j++)
      {
        //m_cells[m_cells[i].getNeighbourCellIDs()[j]];

        // if the neighbour is perpendicular:
        if ( m_cells[m_cells[i].getNeighbourCellIDs()[j]].getCentre().m_x == m_cells[i].getCentre().m_x
             || m_cells[m_cells[i].getNeighbourCellIDs()[j]].getCentre().m_z == m_cells[i].getCentre().m_z)
        {
          //add to perpendicular neighbour of that cell
          m_cells[i].addPerpendicularNeighbourID(m_cells[i].getNeighbourCellIDs()[j]);
        }
      }
    }

    //now we offset the boundries to define playable area
    m_mapBounds.push_back(upperBoundry+m_cellSize*_borderSize);
    m_mapBounds.push_back(lowerBoundry-m_cellSize*_borderSize);
    m_mapBounds.push_back(leftBoundry+m_cellSize*_borderSize);
    m_mapBounds.push_back(rightBoundry-m_cellSize*_borderSize);

    //now we make the vector of exitCells
    for (int i =0; i< m_numberOfCells; i++)
    {
      if (m_cells[i].getCentre().m_z < m_mapBounds[0]
              || m_cells[i].getCentre().m_z > m_mapBounds[1]
              || m_cells[i].getCentre().m_x < m_mapBounds[2]
              || m_cells[i].getCentre().m_x > m_mapBounds[3])
      {
        m_exitPoints.push_back(m_cells[i].getCentre());
      }
    }
}

//----------------------------------------------------------------------------------------------------------------------

CellGraph::~CellGraph()
{
  delete m_entityMgr;
}

//----------------------------------------------------------------------------------------------------------------------

bool CellGraph::entityIsInCell(int _cellID, BaseGameEntity *_entity)
{
  //make floats of cells boundries
  float upper = m_cells[_cellID].getBoundaries().m_x;
  float lower = m_cells[_cellID].getBoundaries().m_y;
  float left  = m_cells[_cellID].getBoundaries().m_z;
  float right = m_cells[_cellID].getBoundaries().m_w;

  //if entity's position is within those boundries
  if(_entity->getPos().m_z >= upper && _entity->getPos().m_z <= lower &&
     _entity->getPos().m_x >= left && _entity->getPos().m_x <= right)
  {
    return true;
  }
  else
  {
    return false;
  }

}

//----------------------------------------------------------------------------------------------------------------------

void CellGraph::initializeCells(BaseGameEntity *_entity)
{
  // for all the cells in the graph
  for (unsigned int i=0; i < m_numberOfCells; i++)
  {
    // if the entity is in the cell
    if (entityIsInCell(i, _entity) == true)
    {
      //add the entity to the cells' list of entities
      m_cells[i].addDynamicEntityID(_entity->getID());
      //set the entity's current cell
      _entity->setCurrentCellID(i);
      _entity->setCurrentCell(m_cells[i]);
      return;
    }
  }
  //else return invalid cell ID
  _entity->setCurrentCellID(-1);
}

//----------------------------------------------------------------------------------------------------------------------

void CellGraph::updateCells(BaseGameEntity *_entity)
{
  //if entity is in the same cell as before the check,
  if (entityIsInCell(_entity->getCurrentCellID(), _entity) == true)
  {
    //add the entity to the cell
    m_cells[_entity->getCurrentCellID()].addDynamicEntityID(_entity->getID());
  }
  else
  {
    // else for all of its previous cells neighbours
    for (int i = 0; i < _entity->getCurrentCell().getNeighbourCellIDs().size(); i++)
    {
      int currentNeighbourCellID = _entity->getCurrentCell().getNeighbourCellIDs()[i];
      //if the entity is in a neighbour
      if (entityIsInCell(currentNeighbourCellID, _entity) == true)
      {
        //add the entity to the neighbour
        m_cells[currentNeighbourCellID].addDynamicEntityID(_entity->getID());
        //set the entity's current cell to the neighbour
        _entity->setCurrentCellID(currentNeighbourCellID);
        _entity->setCurrentCell(m_cells[currentNeighbourCellID]);
      }
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------

void CellGraph::clearCells()
{
  //for all the cells in the graph
  for (unsigned int i = 0; i<m_numberOfCells; i++)
  {
    //clear their list of entities
    m_cells[i].clearDynamicEntityIDs();
  }
}

void CellGraph::addEntities(BaseGameEntity *_entity)
{

  if (_entity >= 0)
  {
    //Clear the entity's vector of dynamic entities
    _entity->clearDetectedDynamicEntityID();
    _entity->clearAgentIDs();

    int numberOfEntiesInCurrentCell = m_cells[_entity->getCurrentCellID()].getDynamicEntityIDs().size();
    // for loop that loops through all entitities in the cell and adds them to the list of dynamic entities of _entity.
    for (unsigned int i = 0; i < numberOfEntiesInCurrentCell; i++)
    {
      //if its not itself
      if (_entity->getID() != m_cells[_entity->getCurrentCellID()].getDynamicEntityIDs()[i])
      {
        //add to entity's dynamic entities list.
        _entity->addDetectedDynamicEntityID(m_cells[_entity->getCurrentCellID()].getDynamicEntityIDs()[i]);
      }

    }

    //Nested for loop that mimices the one above, but adds entities in all of the neighbouring cells.
    int numberOfCellsToCheck = m_cells[(_entity->getCurrentCellID())].getNeighbourCellIDs().size();
    //loops through all neighbour cells
    for (int i = 0; i < numberOfCellsToCheck; i++)
    {
      int currentNeighbourCell = m_cells[(_entity->getCurrentCellID())].getNeighbourCellIDs()[i];
      //Tests all entities in neighbouring cells against the entity's detection radius and adds it to its detected neighbours:
      numberOfEntiesInCurrentCell = m_cells[currentNeighbourCell].getDynamicEntityIDs().size();

      //for loop that checks the distance of entities in neighbour cells
      for (unsigned int i = 0; i < numberOfEntiesInCurrentCell; i++)
      {
        ngl::Vec3 vectorToEntity = _entity->getPos()- (m_entityMgr->getEntityFromID(m_cells[currentNeighbourCell].getDynamicEntityIDs()[i])->getPos());

        //if they are within the entity's detection radius, add to entity's dynamic entity list
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
      if ( m_entityMgr->getEntityFromID(_entity->getDetectedEntityIDs()[i])->getEntityType() == typePolice)
      {
          _entity->addPoliceID(_entity->getDetectedEntityIDs()[i]);
      }
      else if ( m_entityMgr->getEntityFromID(_entity->getDetectedEntityIDs()[i])->getEntityType() == typeRioter)
      {
          _entity->addRioterID(_entity->getDetectedEntityIDs()[i]);
      }
      else if ( m_entityMgr->getEntityFromID(_entity->getDetectedEntityIDs()[i])->getEntityType() == typeStaticEntity)
      {
          _entity->addObstacleID(_entity->getDetectedEntityIDs()[i]);
      }
  }
}

//----------------------------------------------------------------------------------------------------------------------

void CellGraph::generateWalls()
{
  //for all cells in graph, checks perpendicular neighbours and adds walls where there are none
  for (unsigned int i=0; i < m_numberOfCells; i++)
  {
    //if no neighbour centre is detected, the bool remains true:
    bool upperWall = true, lowerWall = true, leftWall =  true,  rightWall= true;


    //check for walls in all four directions, by comparing centre of perpendicular neighbour against cell boundries
    for ( int j =0; j < m_cells[i].getPerpendicularNeighbourCellIDs().size(); j ++)
    {
      ngl::Vec3 currentNeighbourCentre = m_cells[m_cells[i].getPerpendicularNeighbourCellIDs()[j]].getCentre();

      // boundries of cells are stored in the order: upper, lower, left, right
      // so cell.getBoundaries().m_x is the upper boundry etc.

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

    //Now make the walls
    if(upperWall == true)
    {
      //make upper wall
      ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_x);
      ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_x);
      Wall newWall;
      newWall.start =start;
      newWall.end = end;
      newWall.normal = ngl::Vec3(0.0f,0.0f,1.0f);
      m_cells[i].addWallInCell(newWall);

      //push back info into wall centre, normal and rotation vectors
      ngl::Vec3 centre = (start+end)/2.0;
      m_wallCentres.push_back(centre);
      m_wallNormals.push_back(ngl::Vec3(0.0f,0.0f,1.0f));
      m_wallRotations.push_back(-90);
    }

    if(lowerWall == true)
    {
      //make lower wall
      ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_y);
      ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_y);
      Wall newWall;
      newWall.start =start;
      newWall.end = end;
      newWall.normal = ngl::Vec3(0.0f,0.0f,-1.0f);
      m_cells[i].addWallInCell(newWall);

      //push back info into wall centre, normal and rotation vectors
      ngl::Vec3 centre = (start+end)/2.0;
      m_wallCentres.push_back(centre);
      m_wallNormals.push_back(ngl::Vec3(0.0f,0.0f,-1.0f));
      m_wallRotations.push_back(90);
    }

    if(leftWall == true)
    {
      //make left wall
      ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_y);
      ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_z, 0.0f, m_cells[i].getBoundaries().m_x);
      Wall newWall;
      newWall.start =start;
      newWall.end = end;
      newWall.normal = ngl::Vec3(1.0f,0.0f,0.0f);
      m_cells[i].addWallInCell(newWall);

      //push back info into wall centre, normal and rotation vectors
      ngl::Vec3 centre = (start+end)/2.0;
      m_wallCentres.push_back(centre);
      m_wallNormals.push_back(ngl::Vec3(1.0f,0.0f,0.0f));
      m_wallRotations.push_back(0);
    }

    if(rightWall == true)
    {
      //make right wall
      ngl::Vec3 start = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_x);
      ngl::Vec3 end = ngl::Vec3(m_cells[i].getBoundaries().m_w, 0.0f, m_cells[i].getBoundaries().m_y);
      Wall newWall;
      newWall.start =start;
      newWall.end = end;
      newWall.normal = ngl::Vec3(-1.0f,0.0f,0.0f);
      m_cells[i].addWallInCell(newWall);

      //push back info into wall centre, normal and rotation vectors
      ngl::Vec3 centre = (start+end)/2.0;
      m_wallCentres.push_back(centre);
      m_wallNormals.push_back(ngl::Vec3(-1.0f,0.0f,0.0f));
      m_wallRotations.push_back(180);
    }

  }

  //Now append all neighbouring cell walls to each cell, this is done to aid wall avoidance
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

//----------------------------------------------------------------------------------------------------------------------

bool CellGraph::posIsInCell(ngl::Vec3 _pos)
{
  //loops through all cells and checks if a vec3 is in one
  for (int i=0; i<m_numberOfCells; i++)
  {
    float upper = m_cells[i].getBoundaries().m_x;
    float lower = m_cells[i].getBoundaries().m_y;
    float left  = m_cells[i].getBoundaries().m_z;
    float right = m_cells[i].getBoundaries().m_w;

    if(_pos.m_z >= upper && _pos.m_z <= lower &&
       _pos.m_x >= left && _pos.m_x <= right)
    {
      return true;
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------------------------------------

std::vector<ngl::Vec3> CellGraph::findPath(BaseGameEntity *_from, ngl::Vec3 _to)
{
  //we use a method based on dijkstra's algorithm for path planning

  //as we are using cell centres as nodes in paths, we only check perpendicular cells to avoid clipping through corners

  //while current cell != end cell:

  // 1-Update frontier cells:
  // Remove currentCell from frontier.
  // Get perpendicular neighbours of cell. Add them to frontier if they are not in frontier memory.

  // 2-Find closest cell to destination in frontier cells, make this the priorityCell

  // 3- Update search path tree by adding current cell to search path tree:
  // Checks the last SPT in vector of SPTs. From the first element, check if the currentCell is perpendicular.
  // If it is perpendicular to the final element, simply append currentCell to that SPT.
  // If perpendicular to another element, then create a new SPT from the start of the current SPT to the first perpendicular element, append currentCell.
  // If not perpendicular to any elements in the latest SPT, go to the previous SPT and perform check again.
  // This repeats until a perpendicular element is found

  // 4-Go to highest priority cell:
  // Set currentCell to the first element of priorityQueue.


  //vector of vec3s, this is what is returned at the end
  std::vector<ngl::Vec3> finalPath;
  finalPath.clear();

  int endCellID = -1;


  //find the cell that the destination is in
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
  std::cout<<"Failed at finding a path: destination is not in the cellGraph"<<std::endl;
  return finalPath;
  }

  //start cell is the cell that the entity is in
  int startCellID = _from->getCurrentCellID();
  Cell *endCell = &m_cells[endCellID];
  //intitialize current cell
  int currentCellID = startCellID;
  Cell *currentCell = &m_cells[currentCellID];

  //vector of cell IDs to be checked
  std::vector<int> frontierCells;
  //vector of all cell IDs that have been in frontierCells
  std::vector<int> frontierMemory;
  frontierMemory.push_back(startCellID);

  // ID of cell in frontiers that is closest to the destination
  int priorityCellID;

  //temporary search path tree, push back the start cell to initialize
  std::vector<int> newSPT;
  newSPT.push_back(startCellID);

  //vector of search path trees, push back the first SPT
  std::vector<std::vector<int> > SPTs;
  SPTs.push_back(newSPT);

//this is the main path finding loop
  while (currentCellID != endCellID)
  {

    //Remove currentCell from frontierCells
    for (std::vector<int>::iterator iter = frontierCells.begin(); iter != frontierCells.end(); ++iter)
    {
      if( *iter == currentCellID)
      {
        frontierCells.erase(iter);
        break;
      }
    }

    //Add perpendicular neighbours of currentCell to frontierCells if they are not in frontier memory:
    int numberOfPerpendicularNeighbours = currentCell->getPerpendicularNeighbourCellIDs().size();
    for ( int i=0; i <numberOfPerpendicularNeighbours;i++)
    {
      bool isInMemory = false;
      int currentNeighbourID = currentCell->getPerpendicularNeighbourCellIDs()[i];

      //if its in memory, don't add it
      for (int j=0; j<frontierMemory.size();j++)
      {
        if( currentNeighbourID == frontierMemory[j])
        {
          isInMemory = true;
          break;
        }
      }

      if ( isInMemory ==false)
      {
        frontierCells.push_back(currentNeighbourID);
        frontierMemory.push_back(currentNeighbourID);
      }
    }

    // Find the frontier cell that is closest to the destination
    float shortestDist = 10000000000.0f;
    int shortestID;
    int frontierSize = frontierCells.size();
    for (int i=0; i<frontierSize;i++ )
    {
      ngl::Vec3 distance = (m_cells[frontierCells[i]].getCentre() - endCell->getCentre());

      if(distance.lengthSquared() < shortestDist)
      {
        shortestDist = distance.lengthSquared();
        shortestID = frontierCells[i];
      }
    }

    priorityCellID=shortestID;

  //3-Add currentCell to SPT
  //Check the latest value of the latest SPT, if the currentCell is perpendicular then add to that SPT.


    //flag to stop loops after shortest tree has been found
    bool foundShortestTree = false;

    if (currentCellID != startCellID)
    {

      //loops through the SPTs vector starting from the last SPT added
      for (int i = SPTs.size()-1; i >= 0; i--)
      {
        newSPT.clear();

        for ( int j = 0; j < SPTs[i].size(); j++)
        {
          Cell *SPTCell = &m_cells[SPTs[i][j]];

          newSPT.push_back(SPTs[i][j]);

          int SPTNeighboursSize = SPTCell->getPerpendicularNeighbourCellIDs().size();

          for ( int k = 0; k < SPTNeighboursSize; k++)
          {
            //
            if(foundShortestTree == false)
            {
              if(currentCellID == SPTCell->getPerpendicularNeighbourCellIDs()[k])
              {
                newSPT.push_back(currentCellID);
                //if currentCell is perpendicular element in the last SPT
                if(j+1==SPTs[i].size() && i == SPTs.size())
                {
                  SPTs.back() = newSPT;
                  foundShortestTree = true;
                }
                // if currentCell is perpendicular to element in any other SPT, append newSPT
                else
                {
                  SPTs.push_back(newSPT);
                  foundShortestTree = true;
                }
              }
            }
          }
        }
      }
    }

    // go to the next cell
    currentCellID = priorityCellID;
    currentCell = &m_cells[currentCellID];

  }

  // add the end cell to the last SPT
  SPTs.back().push_back(endCellID);


  // use the last SPT to make the finalPath
  for( int i = 0; i< SPTs.back().size(); i++)
  {
    finalPath.push_back(m_cells[SPTs.back()[i]].getCentre());
  }

  // add the accurate destination to the final path
  finalPath.push_back(_to);

  return finalPath;
}
