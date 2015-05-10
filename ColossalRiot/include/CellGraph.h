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
/// @brief This class constructs a graph of cells using an .obj file.
///  this graph is used as a nav mesh, for spatial partitioning and path planning.
/// @author Ben Knowles, Stuart Batchelor
/// @version 1.0
/// @date Last revision 10/05/2015 Updated to comply with doxygen and NCCA coding standard
//----------------------------------------------------------------------------------------------------------------------

class CellGraph
{

public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for cellGraph class
  /// @param [in] _filename name of.obj file to construct graph from.
  /// must be in quads with square faces for all the game calculations to work
  /// @param [in] _bordersize how many cells to leave as a perimeter to game world
  //----------------------------------------------------------------------------------------------------------------------
  CellGraph(const char *_fileName, int _borderSize = 1);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor
  //----------------------------------------------------------------------------------------------------------------------
  ~CellGraph();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns true if entity is in cell, false if it is not
  /// @param [in] _cellID the cell to check
  /// @param [in] _entity the entity to check
  //----------------------------------------------------------------------------------------------------------------------
  bool entityIsInCell(int _cellID, BaseGameEntity *_entity);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns true if position is in the graph, false if it is not
  /// @param [in] _pos the cell to check
  //----------------------------------------------------------------------------------------------------------------------
  bool posIsInCell(ngl::Vec3 _pos);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief finds out what cell an entity is in; goes through all cells in graph
  /// must be called once for each entity to add them to the graph
  /// @param [in] _entity the entity to initialize in the graph
  //----------------------------------------------------------------------------------------------------------------------
  void initializeCells(BaseGameEntity *_entity);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief finds out what cell an entity is in; goes through their last cells neighbour cells
  /// @param [in] _entity the entity to update on the graph
  //----------------------------------------------------------------------------------------------------------------------
  void updateCells(BaseGameEntity *_entity);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clears all entities for each cell in the graph
  //----------------------------------------------------------------------------------------------------------------------
  void clearCells();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds detected entities to given entity.
  /// @param [in] _entity the entity to give detected entities
  //----------------------------------------------------------------------------------------------------------------------
  void addEntities(BaseGameEntity *_entity);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns path to destination
  /// @param [in] _entity entity to find path for
  /// @param [in] _to the position of the destination
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> findPath(BaseGameEntity *_from, ngl::Vec3 _to);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns playable area boundries in order of upper, lower, left then right
  /// @return std::vector<float> playable area boundries in order of upper, lower, left then right
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<float> getMapBounds(){return m_mapBounds;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns a vector of exit points to the map
  /// @return std::vector<ngl::Vec3> of exit points to the map
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> getExitPoints(){return m_exitPoints;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief finds where edges of graph and adds them as Wall structs to cells
  //----------------------------------------------------------------------------------------------------------------------
  void generateWalls();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns vector of all the centres of walls
  /// @return std::vector<ngl::Vec3> of all wall centres
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> getWallCentres(){return m_wallCentres;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns vector of all the normals of walls
  /// @return std::vector<ngl::Vec3> of all wall normals
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> getWallNormals(){return m_wallNormals;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns a vector of all the rotation of walls
  /// @return std::vector<int> of all the rotations of walls
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> getWallRotations(){return m_wallRotations;}

private:

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief pointer to the entity manager
  //----------------------------------------------------------------------------------------------------------------------
  EntityManager* m_entityMgr;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of cells, this is the graph
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <Cell> m_cells;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of exit points of the graph
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <ngl::Vec3> m_exitPoints;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of the graph boundries
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<float> m_mapBounds;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief number of cells in the graph
  //----------------------------------------------------------------------------------------------------------------------
  int m_numberOfCells;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief width of the cells
  //----------------------------------------------------------------------------------------------------------------------
  float m_cellSize;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of centres of all the walls
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> m_wallCentres;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of normals of all the walls
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<ngl::Vec3> m_wallNormals;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief vector of all rotations of all the walls
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> m_wallRotations;

};


#endif // CELL_H
