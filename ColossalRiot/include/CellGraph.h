#ifndef CELLGRAPH_H
#define CELLGRAPH_H

#include <fstream>
#include <sstream>
#include <string>
#include <ngl/Transformation.h>
#include "Cell.h"
#include "EntityManager.h"
#include <ngl/Obj.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file CellGraph.h
/// @brief This class constructs a graph from an .obj file that is used for spatial partitioning and path planning.
/// @author Ben Knowles, Stuart Batchelor
/// @version 1.0
/// @date Last revision
//----------------------------------------------------------------------------------------------------------------------

class CellGraph
{
public:

  CellGraph(const char *_fileName, int _borderSize);
  ~CellGraph();


  bool entityIsInCell(int _cellID, BaseGameEntity *_entity);
  bool posIsInCell(ngl::Vec3 _pos);

  void initializeCells(BaseGameEntity *_entity);
  void updateCells(BaseGameEntity *_entity);
  void clearCells();
  void addEntities(BaseGameEntity *_entity);
  void generateWalls();

  std::vector<ngl::Vec3> getWallCentres(){return m_wallCentres;}
  std::vector<ngl::Vec3> getWallNormals(){return m_wallNormals;}
  std::vector<int> getWallRotations(){return m_wallRotations;}

  std::vector<float> getMapBounds(){return m_mapBounds;}
  std::vector<ngl::Vec3> getExitPoints(){return m_exitPoints;}
  std::vector<ngl::Vec3> findPath(BaseGameEntity *_from, ngl::Vec3 _to);

private:

  EntityManager* m_entityMgr;

  std::vector <Cell> m_cells;
  std::vector <ngl::Vec3> m_exitPoints;
  std::vector<float> m_mapBounds;
  int m_numberOfCells;
  float m_cellSize;
  float m_maxDist; //Maximum distance from the centre of the cell within the cell.
  std::vector<ngl::Vec3> m_wallCentres;
  std::vector<ngl::Vec3> m_wallNormals;
  std::vector<int> m_wallRotations;



};


#endif // CELL_H
