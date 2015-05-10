
#ifndef __CELL_H_
#define __CELL_H_

#include <vector>
#include <list>
#include <glm/glm.hpp>
#include <ngl/Transformation.h>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
/// @brief Wall structure containing the start, end and normal of the wall.
//----------------------------------------------------------------------------------------------------------------------
struct Wall
{
    ngl::Vec3 start;
    ngl::Vec3 end;
    ngl::Vec3 normal;
};


//----------------------------------------------------------------------------------------------------------------------
/// @file Cell.h
/// @brief this the base cell class, used in CellGraph. It encapsulates all entities within it, neighbouring cells and walls as well.
/// @author Ben Knowles & Stuart Batchelor.
/// @version 1.0
/// @date 10/5/15
//----------------------------------------------------------------------------------------------------------------------


class Cell
{
public:
  Cell();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for OpenGL window must set the surface type to OpenGL
  /// @param [in] _ID the Identification int of the cell.
  /// @param [in] _fourCorners the four corners of the square cell.
  /// @param [in] _neighbourCellIDs a list of the IDs of all cells touching this cell, including diagonal corners.
  /// @param [in] _perpendicularCellIDs a list of the IDs of the cells that are perpendicular and touching this cell.
  //----------------------------------------------------------------------------------------------------------------------
  Cell(int _ID, std::vector <ngl::Vec3> _fourCorners, std::vector<int> _neighbourCellIDs,std::vector<int> _perpendicularCellIDs);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief dtor of the cell.
  //----------------------------------------------------------------------------------------------------------------------
  ~Cell();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the ID of the cell.
  /// @return int value of cell ID.
  //----------------------------------------------------------------------------------------------------------------------
  int getID(){return m_ID;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the Centre point of the cell.
  /// @return vec3 value of centre point.
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 getCentre(){return m_centre;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the Centre point of the cell.
  /// @return vec3 value of centre point.
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec4 getBoundaries(){ return m_boundries;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief gets the size of the cell.
  /// @return float value of cell width.
  //----------------------------------------------------------------------------------------------------------------------
  float getSize(){return fabs(m_boundries[0] - m_boundries[1]);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds dynamic entity to cell.
  /// @param [in] _ID ID of dynamic entity to be added.
  //----------------------------------------------------------------------------------------------------------------------
  void addDynamicEntityID(int _ID){ m_dynamicEntityIDs.push_back(_ID);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns dynamic entitys within the cell.
  /// @return vector of IDs of the dynamic entities within the cell.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> getDynamicEntityIDs(){return m_dynamicEntityIDs;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief clears the dynamic entitys within the cell.
  //----------------------------------------------------------------------------------------------------------------------
  void clearDynamicEntityIDs(){m_dynamicEntityIDs.clear();}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds perpendicular neighbour cell ID to the vector of perpendicular neighbourcells.
  /// @param [in] _ID perpedicular neighbour cell ID to be added.
  //----------------------------------------------------------------------------------------------------------------------
  void addPerpendicularNeighbourID(int _ID){m_perpendicularNeighbourCellIDs.push_back(_ID);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns neighbouring cell IDs.
  /// @return vector of IDs of the cells neighbouring this cell.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> getNeighbourCellIDs(){return m_neighbourCellIDs;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns perpendicular neighbouring cells.
  /// @return vector of IDs of the perpendicular neighbours .
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> getPerpendicularNeighbourCellIDs(){ return m_perpendicularNeighbourCellIDs;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds a Wall to the vector of walls.
  /// @param [in] _wall Wall to be pushed back into vector of Walls.
  //----------------------------------------------------------------------------------------------------------------------
  void addWall(const Wall _wall){m_walls.push_back(_wall);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief adds a Wall within the cell to the vector of walls.
  /// @param [in] _wall Wall to be pushed back into the vector of Walls.
  //----------------------------------------------------------------------------------------------------------------------
  void addWallInCell(const Wall _wall){m_wallsInCell.push_back(_wall);}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the vector of Walls.
  /// @return vecotor of Walls around the cell and its neighbours.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Wall> getWalls(){return m_walls;}
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the vector of walls in the cell.
  /// @return vector of walls within the cell.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Wall> getWallsInCell(){return m_wallsInCell;}

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used as the ID of the cell.
  //----------------------------------------------------------------------------------------------------------------------
  int m_ID;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used store the centre of the cell in world space.
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec3 m_centre;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used store the four corners of the cell in world space.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <ngl::Vec3> m_fourCorners;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used store the boundries of the cell in world space. This is ordered upper, lower, left, right.
  //----------------------------------------------------------------------------------------------------------------------
  ngl::Vec4 m_boundries;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used store the dynamic entities within the cell.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<int> m_dynamicEntityIDs;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used store the neighbouring cell IDs.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <int> m_neighbourCellIDs;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used store the perpendicular neighbouring cell IDs.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector <int> m_perpendicularNeighbourCellIDs;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used store the walls within the cell and those of all neighbouring cells.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Wall> m_walls;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief used store the walls within the cell.
  //----------------------------------------------------------------------------------------------------------------------
  std::vector<Wall> m_wallsInCell;

};

#endif // CELL_H
